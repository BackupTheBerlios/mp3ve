;***********************************************************************
;*  MP3VE - Standalone MP3 Player
;*  Copyright (C) 2000,2001,2002 Marc Dressler, Jens Funke,
;*                               Thomas Gutzeit, Nils Steenbock
;*
;*  This program is free software; you can redistribute it and/or modify
;*  it under the terms of the GNU General Public License as published by
;*  the Free Software Foundation; either version 2 of the License, or
;*  (at your option) any later version.
;*
;*  This program is distributed in the hope that it will be useful,
;*  but WITHOUT ANY WARRANTY; without even the implied warranty of
;*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;*  GNU General Public License for more details.
;*
;*  You should have received a copy of the GNU General Public License
;*  along with this program; if not, write to the Free Software
;*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;*
;*  info@mp3ve.de
;***********************************************************************

;***********************************************************************
;* $Id: rpc.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Remote Procedure Call 
;*  Version     : 1.5
;*				
;*  Filename    : rpc.inc
;*  Created     : 13.07.2000 by Nils Steenbock
;*  Last changed: 21.01.2001 by Marc Dressler
;*	
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : communcation interface for mp3ctrlio and mp3ide
;*
;*  Homepage    : www.MP3ve.de
;*                www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : uart.inc
;*		  mp3.inc
;*		  debug.inc
;*		  tocj.inc
;*		  playlist.inc
;*
;***********************************************************************
	
	ifndef	rcpinc		;prevent multiple binding

rcpinc	equ	1

	if       mompass=1
	  message "RPC-Include V1.5 (C) 2000 by Nils Steenbock"
	endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "uart.inc"

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

RPC_Ident1:	 equ	0x43	; "C"  RPC-command identifier "CMD"
RPC_Ident2:	 equ	0x4d	; "M"
RPC_Ident3:	 equ	0x44	; "D"

;RPC_NoCRC:	equ	1	; disable crc-receive-check

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************
	
	SEGMENT	DATA

RPC_Flags:	res	1	; for command received flag

					; RPC_Flags : [default]
					; status flags for rpc support
RPC_FcmdReceived	equ	0	; bit 0 [0] - RPC command received
RPC_F1	equ	1			; bit 1 [0] - 
RPC_F2	equ	2			; bit 2 [0] - 
RPC_F3	equ	3			; bit 3 [0] - 
RPC_F4	equ	4			; bit 4 [0] - 
RPC_F5	equ	5			; bit 5 [0] - 
RPC_F6	equ	6			; bit 6 [0] - 
RPC_F7	equ	7			; bit 7 [0] - 

RPC_State1:	res	1		; state of FSM1
RPC_State2:	res	1		; state of FSM2

RPC_RxDataPtLo:	res	1		; pointer to RPC_RxData0-255
RPC_RxDataPtHi:	res	1

RPC_RxCRC:	res	1	; must be 0 for a valide command

RPC_Rx:				; space to store RPC-Command	
RPC_RxID:	res	1
RPC_RxLen:	res	1
RPC_RxData:	
RPC_RxData0:	res	1
RPC_RxData1:	res	1
RPC_RxData2:	res	1
RPC_RxData3:	res	1
RPC_RxData4:	res	1
RPC_RxData5:	res	1
RPC_RxData6:	res	1
RPC_RxData7:	res	1
RPC_RxData8:	res	1
RPC_RxData9:	res	1
RPC_RxData10:	res	1
RPC_RxData11:	res	1
RPC_RxData12:	res	1
RPC_RxData13:	res	1
RPC_RxData14:	res	1
RPC_RxData15:	res	1	
		res	256-16
RPC_RxDataEnd:		

	
RPC_Tx:		
RPC_TxID:	res	1	; ID of Data-Typ
RPC_TxLen:	res	1	; length of RPC-Tx data package (0=256bytes)
RPC_TxData:			; 256 bytes 
RPC_TxData0:	res	1	; first 16 labels for direct adressing
RPC_TxData1:	res	1
RPC_TxData2:	res	1
RPC_TxData3:	res	1
RPC_TxData4:	res	1
RPC_TxData5:	res	1
RPC_TxData6:	res	1
RPC_TxData7:	res	1
RPC_TxData8:	res	1
RPC_TxData9:	res	1
RPC_TxData10:	res	1
RPC_TxData11:	res	1
RPC_TxData12:	res	1
RPC_TxData13:	res	1
RPC_TxData14:	res	1
RPC_TxData15:	res	1
		res	256-16
RPC_TxDataEnd:
	SEGMENT CODE				     

;***********************************************************************
;* FUNCTION	: RPC_Init
;*              
;* CREATED	: 13.07.2000 by Nils Steenbock
;*              
;* LAST CHANGED : 19.01.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Initializes FSM-state
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Init:
	push	temp1

	ldi 	temp1, 0
	sts	RPC_State1, temp1		; reset FSMs
	sts	RPC_Flags, temp1		; no command received yet

	pop 	temp1	
	ret

;***********************************************************************
;* FUNCTION	: RPC_Transmit
;*              
;* CREATED	: 18.01.2001 by Marc Dressler
;*              
;* LAST CHANGED : 18.01.2001 by Marc Dressler
;*              
;* DESCRIPTION  : send informations from RPC_TxData
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Transmit:
	push	param2
	push	param3
	push	zl
	push	zh
	
	lds	param2, RPC_TxID
	lds	param3, RPC_TxLen	

	ldi	zl, lo(RPC_TxData)
	ldi	zh, hi(RPC_TxData)

	rcall	RPC_TransmitData

	pop	zh
	pop	zl
	pop	param3
	pop	param2
	
	ret

;***********************************************************************
;* FUNCTION	: RPC_TransmitData
;*              
;* CREATED	: 18.01.2001 by Marc Dressler
;*              
;* LAST CHANGED : 21.01.2001 by Marc Dressler
;*              
;* DESCRIPTION  : 
;*              
;* INPUT        : param2 - data-type id
;*		  param3 - data length (0=256 bytes)
;*		  z - pointer to data 	
;*              
;* OUTPUT       : -
;*
;***********************************************************************

RPC_TransmitData:
	push	param1
	push	temp1

	mov	temp1, param2		; init CRC = param2 + param3
	add	temp1, param3
		
	ldi	param1, RPC_Ident1 	; send RPC identifier 
	rcall	UART_Write
	ldi	param1, RPC_Ident2
	rcall	UART_Write
	ldi	param1, RPC_Ident3
	rcall	UART_Write

	mov	param1, param2
	rcall	UART_Write		; send RPC data type id
	
	mov	param1, param3
	rcall	UART_Write		; send RPC data length

RPC_TransmitDataLoop:
	ld	param1, Z+
		
	add	temp1, param1	
	rcall	UART_Write

	dec	param3
	brne	RPC_TransmitDataLoop

	mov	param1, temp1
	neg	param1
	rcall	UART_Write		; write CRC8
	
	pop	temp1
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: RPC_Receive
;*              
;* CREATED	: 13.07.2000 by Nils Steenbock
;*              
;* LAST CHANGED : 19.01.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Interrupt routine to handle incoming bytes from UART
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Receive:
	push 	param1				; save temp values
	push 	temp1				; temp1 = FSM1
	push	temp2
	
	rcall 	UART_Read			; read incoming character
  
	lds	temp1, RPC_Flags
	sbrc	temp1, RPC_fCmdReceived		; still another command in progress ?
	rjmp	RPC_Exit			; then ignore incoming character
	
	lds	temp1, RPC_State1		; get current state of FSM1

	cpi 	temp1, 0			; waiting for RPC_Ident1 ?
	breq 	RPC_Check_Ident1

	cpi 	temp1, 1			; waiting for RPC_Ident2 ?
	breq 	RPC_Check_Ident2
	
	cpi 	temp1, 2			; waiting for RPC_Ident3 ?
	breq 	RPC_Check_Ident3

	lds	temp2, RPC_RxCRC
	add	temp2, param1
	sts	RPC_RxCRC, temp2		; store new calculated checksum

	cpi 	temp1, 3			; waiting for RPC_Id ?
	breq 	RPC_GetID

	cpi 	temp1, 4			; waiting for RPC_Len ?
	breq 	RPC_GetLen

	cpi 	temp1, 5			; waiting for RPC_Data ?
	breq 	RPC_ReceiveData
	
RPC_CrcCheck:
	ldi	temp1, 0			; reset FSM1

  ifndef RPC_NoCRC
	lds	param1, RPC_RxCRC		; load calculated checksum
	cpi	param1,0
	breq	RPC_CRC_Ok
						; invalide checksum !
	rjmp	RPC_exit
  endif
  
RPC_CRC_Ok:
	lds	param1, RPC_Flags
	sbr	param1, (1<<RPC_FcmdReceived)
	sts	RPC_Flags, param1
	
	rjmp	RPC_exit
	
RPC_ReceiveData:
	push	ZL
	push	ZH

	lds	ZL, RPC_RxDataPtLo		; else append character to
	lds	ZH, RPC_RxDataPtHi		; command buffer
	st	Z+, param1
	sts	RPC_RxDataPtLo, ZL
	sts	RPC_RxDataPtHi, ZH

	pop	ZH				
	pop	ZL

	lds	temp2, RPC_State2
	dec	temp2
	breq	RPC_WaitFor_CRC
	sts	RPC_State2, temp2

	rjmp	RPC_exit
	
RPC_WaitFor_CRC:
	inc	temp1	
	rjmp	RPC_exit
	
RPC_Check_Ident1:
	ldi	temp1, 0			; reset FSM1
	cpi 	param1, RPC_Ident1		; got a 'C' ?
	brne 	RPC_exit
	inc	temp1
	rjmp 	RPC_exit
	
RPC_Check_Ident2:
	cpi 	param1, RPC_Ident2		; got an 'M'
	brne 	RPC_Check_Ident1
	inc	temp1
	rjmp	RPC_exit
	
RPC_Check_Ident3:
	cpi 	param1, RPC_Ident3		; got a 'D' ?
	brne 	RPC_Check_Ident1
	inc	temp1

	ldi	temp2, 0
	sts	RPC_RxCRC, temp2		; reset checksum

	rjmp	RPC_exit

RPC_GetID:
	inc	temp1
	sts	RPC_RxID, param1		; store RPC Command ID
	rjmp	RPC_exit

RPC_GetLen:
	inc	temp1
	sts	RPC_RxLen, param1		; store 
	sts	RPC_State2, param1		; data-byte counter 

	ldi	param1, lo(RPC_RxData)		; init data pointer
	sts	RPC_RxDataPtLo, param1
	ldi	param1, hi(RPC_RxData)
	sts	RPC_RxDataPtHi, param1

	rjmp	RPC_exit

RPC_exit:
	sts 	RPC_State1, temp1		; store current state of FSM1

						; restore temp values
	pop 	temp2
	pop 	temp1
	pop	param1
	reti

;***********************************************************************
;* FUNCTION	: RPC_Command_Handler
;*              
;* CREATED	: 13.07.2000 by Nils Steenbock
;*              
;* LAST CHANGED : 19.01.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Command Handler (to be executed in the main loop)
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Command_Handler:
	push	temp1

	lds	temp1, RPC_Flags
	sbrs	temp1, RPC_fCmdReceived		; probe CMDRX-Flag 
	rjmp	RPC_Command_Handler_Exit	; if not set, leave command handler
	
	push	r0				; save temp values
	push	temp2
	push	temp3
	push	temp4
	push 	param1
	push	ZL
	push	ZH
	
	lds	temp1, RPC_RxID			; get command number
	
	cpi	temp1, (((RPC_CommandsEnd-RPC_Commands))-1); is command number out of range ?
	brge	RPC_Clear_CMDRX			; if yes, exit
	
	ldi	ZL, lo(RPC_Commands*2)		; else get command addres from
	ldi	ZH, hi(RPC_Commands*2)		; lookup table

	ldi	temp3,0
	add	ZL, temp1
	adc	ZH, temp3
	add	ZL, temp1
	adc	ZH, temp3
	
	lpm
	mov	temp3, r0
	ldi	temp4, 1
	add	ZL, temp4
	ldi	temp4, 0
	adc	ZH, temp4
	lpm
	mov	temp4, r0
	
	mov	ZL, temp3
	mov	ZH, temp4
	
	icall					; indirect call to RPC-procedure
	
RPC_Clear_CMDRX:
	lds	temp1, RPC_Flags
	cbr	temp1, (1<<RPC_fCmdReceived)	; clear cmd received flag
	sts	RPC_Flags, temp1
	
	pop	ZH				; restore temp values
	pop	ZL
	pop	param1
	pop	temp4
	pop	temp3
	pop	temp2
	pop 	r0

RPC_Command_Handler_Exit:
	pop	temp1
		
	ret

;***********************************************************************

	endif

;*
;* $Log: rpc.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*




