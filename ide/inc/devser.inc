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
;* $Id: devser.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Device Service Functions
;*  Version     : 1.5
;*				
;*  Filename    : devser.inc
;*  Created     : 02/12/2000 by Jens Funke
;*  Last changed: 23/12/2000 by Jens Funke
;*	
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : -
;*
;*  Homepage    : www.MP3ve.de
;*                www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : devdef.inc
;*
;***********************************************************************
	ifndef devserinc
devserinc	=	1

		save
		listing off

                if       mompass=1
                 message "Device Service V1.5 (C) 2000 by Jens Funke"
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
;DEV_Wait_5_Seconds_ID			= 0x0250
;DEV_Increase_Address_ID		= 0x0251
;DEV_Calulate_DevHead_ID		= 0x0252
		
        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: DEV_Wait_5_Seconds
;*              
;* CREATED	: 04/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 04/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Wait 5 seconds
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Wait_5_Seconds:	
	push	param1
	push	param2
	
  ifdef	DEV_Wait_5_Seconds_ID
	DEBUG_MSGID DEV_Wait_5_Seconds_ID
  endif	
	
	; init Timer/Counter1
	ldi	param1,hi(39062)	; set CompareA 
	ldi	param2,lo(39062)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	TIMER1_CK_1024		; start Timer/Counter1 
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand
DEV_Wait_5_Seconds_Loop:
	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    DEV_Wait_5_Seconds_Loop
	
	pop	param2
	pop	param1
	ret
	

;***********************************************************************
;* FUNCTION	: DEV_Increase_Address
;*              
;* CREATED	: 06/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 06/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Increase LBA 
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
DEV_Increase_Address:
	push	temp1

  ifdef	DEV_Increase_Address_ID
	DEBUG_MSGID DEV_Increase_Address_ID
  endif	
	
	ldi	temp1,1
	add	param4,temp1
	brcc	DEV_Increas_Address_End
	add	param3,temp1
	brcc	DEV_Increas_Address_End
	add	param2,temp1
	brcc	DEV_Increas_Address_End
	add	param1,temp1

DEV_Increas_Address_End:
	pop	temp1
	ret
	
;***********************************************************************
;* FUNCTION	: DEV_Calulate_DevHead
;*              
;* CREATED	: 06/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 06/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Calulates the Device/Head reg.
;*              
;* INPUT        : param1	-	bit 24-27 of LBA
;*              
;* OUTPUT       : param1	-	Device/Head reg.
;*
;***********************************************************************
DEV_Calulate_DevHead:
	push	temp1
	push	temp2
	
  ifdef	DEV_Calulate_DevHead_ID
	DEBUG_MSGID DEV_Calulate_DevHead_ID
  endif
	
	mov	temp1,param1			; rescue param1
	andi	temp1,(0x0F)			; i need only the lower nibble
	
	rcall	DEV_GetDrvStatusFlags_CMD
	andi	param1,(1<<DEV_LBA_Mode)	; only bit 6
	
	lds	temp2,DEV_StatusFlags
	andi	temp2,(1<<DEV_DrvSelected)	; only bit 4
	
	or	param1,temp2
	or	param1,temp1	; now create the Device/Head reg.

	pop	temp2
	pop	temp1
	ret
	
;***********************************************************************
		restore
	endif	

;*
;* $Log: devser.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
