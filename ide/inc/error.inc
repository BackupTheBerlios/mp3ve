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
;* $Id: error.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : Error service functions
;*  Version         : 1.5
;*
;*  Filename        : error.inc
;*  Created         : 11.07.2000 by Marc Dressler
;*  Last changed    : 20.01.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : error handling
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : uart.inc
;*		      tocj.inc
;*		      atapi.inc
;*		      mp3.inc
;*
;***********************************************************************

	ifndef	errorinc	; prevent multiple binding

errorinc	equ	1

		if	mompass=1
                 message "Error Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "uart.inc"
;	INCLUDE "tocj.inc"
	INCLUDE "atapi.inc"
		ifndef mp3_disabled
	INCLUDE "mp3.inc"
		endif

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

	SEGMENT DATA

Error:		res	1	; Error Byte:
				; byte = 0 - no error
				; bit 0 -
				; bit 1 -
				; bit 2 -
				; bit 3 -
				; bit 4 -
				; bit 5 -
				; bit 6 -
				; bit 7 -

				; Error-Flags
Error_fTOC	equ	0	; see TOC_ErrorFlags for additional information
Error_fDEV	equ	1	; see DEV_xxxFlags for additional information
Error_fMP3	equ	2	; see MP3_ErrorFlags for additional information
Error_f3	equ	3
Error_f4	equ	4
Error_f5	equ	5
Error_f6	equ	6
Error_f7	equ	7


        SEGMENT CODE


;***********************************************************************
;* FUNCTION	: Error_Init
;*
;* CREATED	: 11.07.2000 by Marc Dressler
;*
;* LAST CHANGED : 27.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : init error flags
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Error_Init:
	push	temp1
	ldi	temp1, 0x00		; clear errors
	sts	Error, temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: Error_Service
;*
;* CREATED	: 11.07.2000 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : check and write error to uart
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Error_Service:
	push	param1

	lds	param1, Error
	cpi	param1, 0x00
	breq	Error_None
Error_present:
;	ldi	param1, 'E'		; E = error (preversion)
;	rcall	UART_Write

;	lds	param1, TOC_ErrorFlags
;	rcall	UART_WriteHex

;	lds	param1, ATAPI_ErrorFlags
;	rcall	UART_WriteHex

;	lds	param1, MP3_ErrorFlags
;	rcall	UART_WriteHex

;	rcall	UART_WriteCRLF

	ldi	param1, 0x00		; clear errors
	sts	Error, param1

Error_None:

	ifndef mp3_disabled 
Error_CheckStatus:
	; check if mp3 file-end reached
	sbrs	MP3_Flags, MP3_Ffileend	; end reached ?
	rjmp	Error_NoFileEnd

  ifdef rpc_old
	UART_PRINT "MSG"		; send "file-end reached"-message
  endif
  
  ifndef rpc_old
	ldi	param1, RPC_CMD_Stop
	sts	RPC_TxID, param1

	ldi	param1, 1
	sts	RPC_TxLen, param1 	; 1 return byte (RPC_TxData0) 

	rcall	RPC_Transmit
  endif
	cbr	MP3_Flags, (1<<MP3_Ffileend)	; clear flag

Error_NoFileEnd:
	cbr	MP3_Flags, (1<<MP3_Finvalide)	; clear flag

	pop	param1
	ret

	endif
;***********************************************************************

	endif

;*
;* $Log: error.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
