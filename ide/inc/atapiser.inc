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
;* $Id: atapiser.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : ATAPIService
;*  Version         : 1.5
;*
;*  Filename        : atapiser.asm
;*  Created         : 23.04.2000 by Jens Funke
;*  Last changed    : 07.04.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : Service Functions for ATAPI
;*
;*  Homepage        : www.MP3ve.de
;*		      www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : uart.inc
;*		      atapi.inc
;*		      atapireg.inc
;*		      regavr.inc
;*		      register.inc
;*
;***********************************************************************
	ifndef atapiserinc
atapiserinc	=	1

		save
;		listing off

                if       mompass=1
                 message "ATAPI Service V1.5 (C) 2000 by Jens Funke"
                endif


        INCLUDE "atapidef.inc"

;***********************************************************************
;* Debug IDs
;***********************************************************************
;*
;* To enable the output of a debug message define the message ID below.
;* Comment out message IDs to disable a debug message.
;*
;***********************************************************************
;	
;ATAPI_Wait_NBSY_ID			= 0x0460
;ATAPI_Wait_NBSY_Timeout_ID		= 0x0461
	
;ATAPI_Wait_INTRQ_ID			= 0x0462
;ATAPI_Wait_INTRQ_Timeout_ID		= 0x0463
	
;ATAPI_Read_Buffer_Once_ID		= 0x0464
;ATAPI_Write_Buffer_ID			= 0x0465
;ATAPI_Read_SenseKey_ID			= 0x0466
;ATAPI_XCopy_Buffer2SRAM_ID		= 0x0467
;ATAPI_Identify_Packet_Device_ID	= 0x0468
;ATAPI_Read_Inquiry_Data_ID		= 0x0469
;_ID			= 0x0470
;_ID			= 0x0471
;ATAPI_Determine_MediumType_ID		= 0x0472			
;ATAPI_Evaluate_ASC_ID			= 0x0473
	
	
		
        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: ATAPI_Wait_NBSY
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 10.08.2000 by Jens Funke
;*
;* DESCRIPTION  : read StatusByte and wait for BSY=0
;*
;* INPUT        : -
;*
;* OUTPUT       : Set DEV_TimeoutError bit in DEV_xxxErrorFlags if timer overflow occurs
;*
;***********************************************************************
ATAPI_Wait_NBSY:
	push	param1
	push	param2
	
  ifdef	ATAPI_Wait_NBSY_ID
	DEBUG_MSGID ATAPI_Wait_NBSY_ID
  endif	
	
        lds     param1,ATAPI_RAStatus    ; read StatusByte and leave if BSY=0
        sbrs    param1,ATAPI_BSY
	rjmp	ATAPI_Wait_NBSY_End
	
	rcall	DEV_GetErrorFlags_CMD	; if we had a Timeout then leave
	sbrc	param1,DEV_TimeoutError	
	rjmp	ATAPI_Wait_NBSY_End
	
	; init Timer/Counter1
	ldi	param1,hi(ATAPI_TIME_NBSY)	; set CompareA (see atapidef.inc)
	ldi	param2,lo(ATAPI_TIME_NBSY)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATAPI_Source_NBSY	; start Timer/Counter1 (see atapidef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand

ATAPI_Wait_NBSY_Loop:
	lds     param1,ATAPI_RAStatus
        sbrs    param1,ATAPI_BSY		; check BSY bit
	rjmp	ATAPI_Wait_NBSY_End

	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    ATAPI_Wait_NBSY_Loop
;        rjmp	ATAPI_Wait_NBSY_Error

ATAPI_Wait_NBSY_Error:
	rcall	DEV_GetErrorFlags_CMD		
	sbr	param1,(1<<DEV_TimeoutError)	; set Timeout-bit in DEV_ErrorFlags
	rcall	DEV_SetErrorFlags_CMD
	
  ifdef	ATAPI_Wait_NBSY_Timeout_ID
	DEBUG_MSGID ATAPI_Wait_NBSY_Timeout_ID
  endif	
	
ATAPI_Wait_NBSY_End:
	pop	param2
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Wait_INTRQ
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 10.08.2000 by Jens Funke
;*
;* DESCRIPTION  : read PORTD and wait for INTRQ=1 (PD2)
;*
;* INPUT        : -
;*
;* OUTPUT       : Set DEV_TimeoutError bit in DEV_xxxErrorFlags if timer overflow occurs
;*
;***********************************************************************
ATAPI_Wait_INTRQ:
	push	param1
	push	param2
	
  ifdef	ATAPI_Wait_INTRQ_ID
	DEBUG_MSGID ATAPI_Wait_INTRQ_ID
  endif	
	
	sbic	pinb,2			; leave if Bit2 in PORTB is set
        rjmp    ATAPI_Wait_INTRQ_End
	
	rcall	DEV_GetErrorFlags_CMD	; if we had a Timeout then leave
	sbrc	param1,DEV_TimeoutError	
	rjmp	ATAPI_Wait_INTRQ_End
	
	; init Timer/Counter1
	ldi	param1,hi(ATAPI_TIME_INTRQ)	; set CompareA (see atapidef.inc)
	ldi	param2,lo(ATAPI_TIME_INTRQ)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATAPI_Source_INTRQ	; start Timer/Counter1 (see atapidef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand

ATAPI_Wait_INTRQ_Loop:
	sbic	pinb,2			; leave if Bit2 in PORTB is set
        rjmp    ATAPI_Wait_INTRQ_End

	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    ATAPI_Wait_INTRQ_Loop
;        rjmp	ATAPI_Wait_INTRQ_Error

ATAPI_Wait_INTRQ_Error:
	rcall	DEV_GetErrorFlags_CMD		
	sbr	param1,(1<<DEV_TimeoutError)	; set Timeout-bit in DEV_ErrorFlags
	rcall	DEV_SetErrorFlags_CMD
	
  ifdef	ATAPI_Wait_INTRQ_Timeout_ID
	DEBUG_MSGID ATAPI_Wait_INTRQ_Timeout_ID
  endif	
	
ATAPI_Wait_INTRQ_End:
	pop	param2
	pop	param1
	ret


;***********************************************************************
;* FUNCTION	: ATAPI_Read_Buffer_Once
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 23.04.2000 by Jens Funke
;*
;* DESCRIPTION  : Read device buffer and write it to SRAM
;*
;* INPUT        : Z must points to the SRAM-Buffer
;*
;* OUTPUT       : SRAM-Buffer
;*
;***********************************************************************
ATAPI_Read_Buffer_Once:
	push	param1
	push	XL
	push	XH
	
  ifdef	ATAPI_Read_Buffer_Once_ID
	DEBUG_MSGID ATAPI_Read_Buffer_Once_ID
  endif	
	
; First we read the Cylinder Register, because it containts the value of
; bytes to be transfered
	lds	XL,ATAPI_RWACylLow
	lds	XH,ATAPI_RWACylHigh

ATAPI_Read_Buffer_Once_Loop:
        lds     param1,ATAPI_RWADataLow    	; read IDE-Cache
        st      Z+,param1
	
;		ifdef	atapi_debug_high
;	rcall	UART_WriteHex
;		endif
	
        lds     param1,ATAPI_RWADataHigh	 	; read IDE-Cache
        st      Z+,param1
	
;		ifdef	atapi_debug_high
;	rcall	UART_WriteHex
;		endif
	
        sbiw	XL,2				; X-2 because we read a word
        CPIW	XL,XH,0,ATAPI_Read_Buffer_Once_Loop
	
	pop	XH
	pop	XL
	pop 	param1
        ret

  ifdef all_code	

;***********************************************************************
;* FUNCTION	: ATAPI_Write_Buffer  
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 23.04.2000 by Jens Funke
;*
;* DESCRIPTION  : Write SRAM to IDE-Interface
;*
;* INPUT        : Z must point to the SRAM-Buffer
;*
;* OUTPUT       : -
;*
;***********************************************************************
;ATAPI_Write_Buffer:
;	push	temp1
;	
;  ifdef	ATAPI_Write_Buffer_ID
;	DEBUG_MSGID ATAPI_Write_Buffer_ID
;  endif	
;	
;ATAPI_Write_Buffer_Loop
;        ld	temp1,Z+	    ;read IDE-Cache
;        sts     ATAPI_RWADataHigh,temp1
;        ld      temp1,Z+
;        sts     ATAPI_RWADataLow,temp1
;;**** Abbruchbedingung ist noch nicht OK
;        cpi     ZH,0x05                 ;end after 512 bytes
;        brlo    ATAPI_Write_Buffer_Loop
;        pop 	temp1
;        ret

  endif	
;***********************************************************************
;* FUNCTION	: ATAPI_Read_SenseKey
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Read the Sense Key and write it to ATAPI_SenseKey
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_Read_SenseKey:
	push	param1
	push	ZL
	push	ZH
	
  ifdef	ATAPI_Read_SenseKey_ID
	DEBUG_MSGID ATAPI_Read_SenseKey_ID
  endif	

	SET_ZPOINTER	ATAPI_SenseKey		; set Z-Pointer
	ldi	param1,18
	rcall	ATAPI_XCopy_Buffer2SRAM

	pop	ZH
	pop	ZL
	pop 	param1
        ret

  ifdef	atapi_debug_high
;***********************************************************************
;* FUNCTION	: ATAPI_Print_SenseKey
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Print the Sense Key to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_Print_SenseKey:
	push	param1
	push	temp2
	push	ZL
	push	ZH

	SET_ZPOINTER	ATAPI_SenseKey
	ldi	temp2,18			; we need only the first 18 bytes
ATAPI_Print_SenseKey_Loop:
	ld	param1,Z+
	rcall	UART_WriteHex			; print the Sense Key
        sbci	temp2,1
	cpi	temp2,0
	brne	ATAPI_Print_SenseKey_Loop

	pop	ZH
	pop	ZL
	pop	temp2
	pop	param1
	ret
  endif

;***********************************************************************
;* FUNCTION	: ATAPI_XCopy_Buffer2SRAM
;*
;* CREATED	: 14/07/2000 by Jens Funke
;*
;* LAST CHANGED : 14/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Copies X bytes form ATAPI-Buffer to SRAM
;*
;* INPUT        : Z must points to SRAM
;*		  param1	= x
;*
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_XCopy_Buffer2SRAM:
	push	temp1
	push	param1
	
  ifdef	ATAPI_XCopy_Buffer2SRAM_ID
	DEBUG_MSGID ATAPI_XCopy_Buffer2SRAM_ID
  endif	

ATAPI_XCopy_Buffer2SRAM_Loop:
        lds     temp1,ATAPI_RWADataLow    	; read ATAPI-Cache
        st      Z+,temp1
        lds     temp1,ATAPI_RWADataHigh	 	; read ATAPI-Cache
        st      Z+,temp1
        sbci	param1,2			; param1 - 2 because we read a word
	cpi	param1,0
	brne	ATAPI_XCopy_Buffer2SRAM_Loop

	rcall	ATAPI_ClearBuffer		; DRQ should be cleared after read form buffer

	pop	param1
	pop 	temp1
        ret

;***********************************************************************
;* FUNCTION	: ATAPI_Identify_Packet_Device
;*
;* CREATED	: 14/07/2000 by Jens Funke
;*
;* LAST CHANGED : 12/08/2000 by Jens Funke
;*
;* DESCRIPTION  : Stores the Identify Packet Device data at Z
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_Identify_Packet_Device:
	push	temp1
	push	ZH
	push	ZL
	
  ifdef	ATAPI_Identify_Packet_Device_ID
	DEBUG_MSGID ATAPI_Identify_Packet_Device_ID
  endif	

	ldi	temp1,ATAPI_IDDev		; send Identify Packet Device
	sts	ATAPI_WACommand,temp1
	
	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected
	rjmp	ATAPI_Identify_Packet_Device_Master
;	rjmp	ATAPI_Identify_Packet_Device_Slave
	
ATAPI_Identify_Packet_Device_Slave:			; if Slave
	SET_ZPOINTER	DEV_Identify_Data
	rjmp	ATAPI_Identify_Packet_Device_Read
	
ATAPI_Identify_Packet_Device_Master:			; if Master 
	SET_ZPOINTER	DEV_Identify_Data
;	rjmp	ATAPI_Identify_Packet_Device_Read
	
ATAPI_Identify_Packet_Device_Read:		
	rcall	ATAPI_Wait_NBSY
	rcall	ATAPI_Read_Buffer_Once
	
	pop	ZL
	pop	ZH
	pop	temp1
	ret
	
;***********************************************************************
;* FUNCTION	: ATAPI_Read_Inquiry_Data
;*
;* CREATED	: 02/12/2000 by Jens Funke
;*
;* LAST CHANGED : 02/12/2000 by Jens Funke
;*
;* DESCRIPTION  : Stores Inquiry data at DEV_xxxInquiry_Data
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_Read_Inquiry_Data:
	push	param1
	push	ZL
	push	ZH
	
  ifdef	ATAPI_Read_Inquiry_Data_ID
	DEBUG_MSGID ATAPI_Read_Inquiry_Data_ID
  endif	

	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_DrvSelected		; Master or Slave 
	rjmp	ATAPI_Read_Inquiry_Data_Master
;	rjmp	ATAPI_Read_Inquiry_Data_Slave
	
ATAPI_Read_Inquiry_Data_Slave:			; if Slave
	SET_ZPOINTER	DEV_Inquiry_Data
	rjmp	ATAPI_Read_Inquiry_Data_XCopy
	
ATAPI_Read_Inquiry_Data_Master:			; if Master 
	SET_ZPOINTER	DEV_Inquiry_Data
;	rjmp	ATAPI_Read_Inquiry_Data_XCopy
	
ATAPI_Read_Inquiry_Data_XCopy:		
	ldi	param1,36
	rcall	ATAPI_XCopy_Buffer2SRAM

	pop	ZH
	pop	ZL
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Determine_MediumType
;*              
;* CREATED	: 11/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 11/08/2000 by Jens Funke
;*              
;* DESCRIPTION  : Find out the kind of the medium. The access to the
;*		  medium must be possible, so check it first.
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_Audio_Medium: 0 = Data, 1 = Audio
;*
;***********************************************************************
ATAPI_Determine_MediumType:
	push	param1
	push	param2
	push	param3
	push	param4
	push	ZH
	push	ZL
	
  ifdef	ATAPI_Determine_MediumType_ID
	DEBUG_MSGID ATAPI_Determine_MediumType_ID
  endif	

	clr	param1
	clr	param2
	clr	param3
	clr	param4
	SET_ZPOINTER DEV_Null_Sector		; write to /dev/null
	rcall	ATAPI_Read10_CMD		; try to read
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1,0
	breq	ATAPI_Determine_MediumType_Data	
	rcall	DEV_GetDrvStatusFlags_CMD	; set DEV_Audio_Medium
	sbr	param1,(1<<DEV_Audio_Medium)	; in DEV_xxxDrvStatusFlags
	rcall	DEV_SetDrvStatusFlags_CMD
	
	rcall	ATAPI_CheckCondition_CMD	; clear errors
	rjmp	ATAPI_Determine_MediumType_End
	
ATAPI_Determine_MediumType_Data:	
	rcall	DEV_GetDrvStatusFlags_CMD	; clear DEV_Audio_Medium	
	cbr	param1,(1<<DEV_Audio_Medium)	; in DEV_xxxDrvStatusFlags
	rcall	DEV_SetDrvStatusFlags_CMD
	
ATAPI_Determine_MediumType_End:	
	pop	ZL
	pop	ZH
	pop	param4
	pop	param3
	pop	param2
	pop	param1
	ret		
			
;***********************************************************************
;* FUNCTION	: ATAPI_Evaluate_ASC
;*              
;* CREATED	: 11/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 11/08/2000 by Jens Funke
;*              
;* DESCRIPTION  : Evaluates ASC and ASCQ from Sense Key
;*		  and set DEV_DrvStatusFlags
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_Evaluate_ASC:
	push	temp1
	push	param1
	push	param2
	
	lds	param1, (ATAPI_SenseKey+12)	; ASC additional Sense Code
	mov	temp1,param1			
	lds	param2, (ATAPI_SenseKey+13)	; ASCQ additional Sense Code
	
  ifdef	ATAPI_Evaluate_ASC_ID
	DEBUG_MSGID ATAPI_Evaluate_ASC_ID
  endif	

	rcall	DEV_GetDrvStatusFlags_CMD	; load DEV_xxxDrvStatusFlags to param1
		
	cpi	temp1,0x28			; evaluate ASC and ASCQ from Sense Key 
	breq	ATAPI_Evaluate_ASC_28_29	; and set DEV_xxxDrvStatusFlags
	cpi	temp1,0x3A
	breq	ATAPI_Evaluate_ASC_3A_05
	cpi	temp1,0x30
	breq	ATAPI_Evaluate_ASC_53_02_30
	cpi	temp1,0x53
	breq	ATAPI_Evaluate_ASC_53
	rjmp	ATAPI_Evaluate_ASC_End
	
ATAPI_Evaluate_ASC_28_29:	
	sbr	param1,(1<<DEV_Medium_Changed)	; set DEV_Medium_Changed 
	rcall	DEV_SetDrvStatusFlags_CMD
	rjmp	ATAPI_Evaluate_ASC_End
ATAPI_Evaluate_ASC_3A_05:
	sbr	param1,(1<<DEV_Medium_Changed)	; set DEV_Medium_Changed  
	cbr	param1,(1<<DEV_Disc_Loaded)	; set DEV_Disc_Loaded
	rcall	DEV_SetDrvStatusFlags_CMD
	rjmp	ATAPI_Evaluate_ASC_End
ATAPI_Evaluate_ASC_53:
	cpi	param2,0x02
	breq	ATAPI_Evaluate_ASC_53_02_30	; check ASCQ 
	rjmp	ATAPI_Evaluate_ASC_End
ATAPI_Evaluate_ASC_53_02_30:	
	cbr	param1,(1<<DEV_Disc_Loaded)	; set DEV_Disc_Loaded
	rcall	DEV_SetDrvStatusFlags_CMD
	rjmp	ATAPI_Evaluate_ASC_End

ATAPI_Evaluate_ASC_End:	
	pop	param2
	pop	param1
	pop	temp1				
	ret
	
;
;
;
;
;ATAPI_Write_Packet12
;ATAPI_Write_Packet16
;ATAPI_Write_Feature
;ATAPI_Write_DevControl
;
;
;


;***********************************************************************
		restore
	endif
;*
;* $Log: atapiser.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
