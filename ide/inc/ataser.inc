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
;* $Id: ataser.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: ATA Service Functions
;*  Version     : 1.5
;*				
;*  Filename    : ataser.inc.asm
;*  Created     : 29/11/2000 by Jens Funke
;*  Last changed: 07.04.2001 by Marc Dressler
;*	
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : -
;*
;*  Homepage    : www.MP3ve.de
;*		  www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : -
;*
;***********************************************************************
   	ifndef  ataserinc       ; prevent multiple binding
ataserinc       equ     1

		save
                ;listing off

                if       mompass=1
                 message "ATA Definitions V1.5 (C) 2000 by Jens Funke"
                endif
	
;***********************************************************************
;* Debug IDs
;***********************************************************************
;*
;* To enable the output of a debug message define the message ID below.
;* Comment out message IDs to disable a debug message.
;*
;***********************************************************************
;	
;ATA_Wait_NBSY_ID			= 0x0350
;ATA_Wait_NBSY_Timeout_ID		= 0x0351
;ATA_Wait_DRDY_ID			= 0x0352
;ATA_Wait_DRDYTimeout_ID		= 0x0353
;ATA_Wait_INTRQ_ID			= 0x0354
;ATA_Read_Buffer_Once_ID		= 0x0355
;ATA_ATAPISector2ATASector_ID		= 0x0356
				

	SEGMENT CODE
	
;***********************************************************************
;* FUNCTION	: ATA_Wait_NBSY
;*              
;* CREATED	: 30/11/2000 by Jens Funke
;*              
;* LAST CHANGED : 30/11/2000 by Jens Funke
;*              
;* DESCRIPTION  : read StatusByte and wait for BSY=0
;*              
;* INPUT        : -
;*              
;* OUTPUT       : Set DEV_TimeoutError bit in DEV_xxxErrorFlags if 
;*		  timer overflow occurs
;*
;***********************************************************************
ATA_Wait_NBSY:
	push	param1
	push	param2
	
  ifdef	ATA_Wait_NBSY_ID
	DEBUG_MSGID ATA_Wait_NBSY_ID
  endif	
	
	lds     param1,ATA_RAStatus      ;read StatusByte and wait for BSY=0
        sbrs    param1,ATA_BSY
	rjmp	ATA_Wait_NBSY_End
	
	rcall	DEV_GetErrorFlags_CMD	; if we had a Timeout then leave
	sbrc	param1,DEV_TimeoutError	
	rjmp	ATA_Wait_NBSY_End

	; init Timer/Counter1
	ldi	param1,hi(ATA_TIME_NBSY)	; set CompareA (see atadef.inc)
	ldi	param2,lo(ATA_TIME_NBSY)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATA_Source_NBSY		; start Timer/Counter1 (see atadef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand

ATA_Wait_NBSY_Loop:
	lds     param1,ATA_RAStatus
        sbrs    param1,ATA_BSY		; check BSY bit
	rjmp	ATA_Wait_NBSY_End
	
	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    ATA_Wait_NBSY_Loop
;        rjmp	ATA_Wait_NBSY_Error
	
ATA_Wait_NBSY_Error:
	rcall	DEV_GetErrorFlags_CMD		
	sbr	param1,(1<<DEV_TimeoutError)	; set Timeout-bit in DEV_ErrorFlags
	rcall	DEV_SetErrorFlags_CMD

  ifdef	ATA_Wait_NBSY_Timeout_ID
	DEBUG_MSGID ATA_Wait_NBSY_Timeout_ID
  endif	
	
ATA_Wait_NBSY_End:		
	pop	param2
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATA_Wait_DRDY
;*              
;* CREATED	: 30/11/2000 by Jens Funke
;*              
;* LAST CHANGED : 30/11/2000 by Jens Funke
;*              
;* DESCRIPTION  : Read StatusByte and wait for DRDY=1
;*              
;* INPUT        : -
;*              
;* OUTPUT       : Set DEV_TimeoutError bit in DEV_xxxErrorFlags if 
;*		  timer overflow occurs
;*
;***********************************************************************
ATA_Wait_DRDY:
	push	param1
	push	param2
	
  ifdef	ATA_Wait_DRDY_ID
	DEBUG_MSGID ATA_Wait_DRDY_ID
  endif	
	
        lds     param1,ATA_RAStatus      ;read StatusByte and wait for DRDY=1
        sbrc    param1,ATA_DRDY
	rjmp	ATA_Wait_DRDY_End
	
	rcall	DEV_GetErrorFlags_CMD	; if we had a Timeout then leave
	sbrc	param1,DEV_TimeoutError	
	rjmp	ATA_Wait_DRDY_End
	
	; init Timer/Counter1
	ldi	param1,hi(ATA_TIME_DRDY)	; set CompareA (see atapef.inc)
	ldi	param2,lo(ATA_TIME_DRDY)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATA_Source_DRDY		; start Timer/Counter1 (see atadef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand

ATA_Wait_DRDY_Loop:
	lds     param1,ATA_RAStatus
        sbrc    param1,ATA_DRDY		; check DRDY bit
	rjmp	ATA_Wait_DRDY_End

	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    ATA_Wait_DRDY_Loop
;        rjmp	ATA_Wait_DRDY_Error

ATA_Wait_DRDY_Error:
	rcall	DEV_GetErrorFlags_CMD		
	sbr	param1,(1<<DEV_TimeoutError)	; set Timeout-bit in DEV_ErrorFlags
	rcall	DEV_SetErrorFlags_CMD
	
  ifdef	ATA_Wait_DRDY_Timeout_ID
	DEBUG_MSGID ATA_Wait_DRDY_Timeout_ID
  endif	

ATA_Wait_DRDY_End:
	pop	param2
	pop	param1
	ret


;***********************************************************************
;* FUNCTION	: ATA_Wait_INTRQ
;*              
;* CREATED	: 30/11/2000 by Jens Funke
;*              
;* LAST CHANGED : 30/11/2000 by Jens Funke
;*              
;* DESCRIPTION  : Read PORTD and wait for INTRQ=1 (PD2)
;*              
;* INPUT        : -
;*              
;* OUTPUT       : Set DEV_TimeoutError bit in DEV_xxxErrorFlags if 
;*		  timer overflow occurs
;*
;***********************************************************************
ATA_Wait_INTRQ:	
	push	param1
	push	param2
	
  ifdef	ATA_Wait_INTRQ_ID
	DEBUG_MSGID ATA_Wait_INTRQ_ID
  endif	
	
	sbic	pinb,2			; leave if Bit2 in PORTB is set
        rjmp    ATA_Wait_INTRQ_End
	
	rcall	DEV_GetErrorFlags_CMD	; if we had a Timeout then leave
	sbrc	param1,DEV_TimeoutError	
	rjmp	ATA_Wait_INTRQ_End
	
	; init Timer/Counter1
	ldi	param1,hi(ATA_TIME_INTRQ)	; set CompareA (see atadef.inc)
	ldi	param2,lo(ATA_TIME_INTRQ)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATA_Source_INTRQ	; start Timer/Counter1 (see atadef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand

ATA_Wait_INTRQ_Loop:
	sbic	pinb,2			; leave if Bit2 in PORTB is set
        rjmp    ATA_Wait_INTRQ_End

	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    ATA_Wait_INTRQ_Loop
;        rjmp	ATA_Wait_INTRQ_Error

ATA_Wait_INTRQ_Error:
	rcall	DEV_GetErrorFlags_CMD		
	sbr	param1,(1<<DEV_TimeoutError)	; set Timeout-bit in DEV_ErrorFlags
	rcall	DEV_SetErrorFlags_CMD
	
  ifdef	ATA_Wait_INTRQ_Timeout_ID
	DEBUG_MSGID ATA_Wait_INTRQ_Timeout_ID
  endif
	
ATA_Wait_INTRQ_End:
	pop	param2
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATA_Read_Buffer_Once
;*              
;* CREATED	: 29/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 29/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Read device buffer and write it to SRAM
;*		  ACHTUNG! Differ from ATAPI_Read_Buffer_Once.
;*			   See the specification.
;*              
;* INPUT        : Z must points to the SRAM-Buffer
;*              
;* OUTPUT       : SRAM-Buffer
;*
;***********************************************************************
ATA_Read_Buffer_Once:
	push	temp1
	push	param1
	
  ifdef	ATA_Read_Buffer_Once_ID
	DEBUG_MSGID ATA_Read_Buffer_Once_ID
  endif
	
ATA_Read_Buffer_Once_Loop:	
	rcall   ATA_Wait_NBSY
	
	lds	temp1,ATA_RAStatus
	sbrs    temp1,ATA_DRQ			; if DRQ = 0 then leave
	rjmp	ATA_Read_Buffer_Once_End
	
        lds     param1,ATA_RWADataLow		; read DataLow Register
	st	Z+,param1
;		ifdef	ata_debug_high
;	rcall	UART_WriteHex
;		endif
	
        lds     param1,ATA_RWADataHigh		; read DataHigh Register
	st	Z+,param1
;		ifdef	ata_debug_high
;	rcall	UART_WriteHex
;		endif
	rjmp	ATA_Read_Buffer_Once_Loop
	
ATA_Read_Buffer_Once_End:
	pop	param1
	pop	temp1
	ret		
		
;***********************************************************************
;* FUNCTION	: ATA_ATAPISector2ATASector
;*              
;* CREATED	: 02/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 07.04.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Calculate ATAPI_Sector=(ATA_Sector+x)*4
;*              
;* INPUT        : param1 = 	LBA (MSB)
;*		  param2 =	LBA
;*		  param3 =	LBA
;*		  param4 =	LBA (LSB)
;*              
;* OUTPUT       : param1 = 	LBA (MSB)
;*		  param2 =	LBA
;*		  param3 =	LBA
;*		  param4 =	LBA (LSB)
;*
;***********************************************************************
ATA_ATAPISector2ATASector:
  ifdef	ATA_ATAPISector2ATASector_ID
	DEBUG_MSGID ATA_ATAPISector2ATASector_ID
  endif
	
  ifdef ata_offset
	push	temp1
	ldi	temp1,0				; now add x=0
	add	param4,temp1
	clr	temp1			
	adc	param3,temp1
	adc	param2,temp1
	adc	param1,temp1
	pop	temp1
  endif
	
	lsl	param4
	rol	param3
	rol	param2
	rol	param1

	lsl	param4
	rol	param3
	rol	param2
	rol	param1

	ret


;***********************************************************************
		restore
	endif
	
;ATA_Write_Buffer	
;ATA_XCopy_Buffer2SRAM
;ATA_Identify_Device

;*
;* $Log: ataser.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
