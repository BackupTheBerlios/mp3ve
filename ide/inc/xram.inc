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
;* $Id: xram.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : External RAM Include-File
;*  Version         : 1.5
;*
;*  Filename        : xram.inc
;*  Created         : 27.11.1999 by Marc Dressler
;*  Last changed    : 13.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : all
;*  Description     : external RAM/ROM service routines
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************
	ifndef	xraminc
xraminc		=	1

                if       mompass=1
                 message "ExRAM Include-File V1.5 (C) 2000 by Marc Dressler"
                endif

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

        SEGMENT CODE
        
;***********************************************************************
;* FUNCTION	: XRAM_Init
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 13.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : -
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

XRAM_Init:      
	rcall   XRAM_On
	rcall   XRAM_WaitStateOff
	ret

;***********************************************************************
;* FUNCTION	: XRAM_On
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 02.12.2000 by Jens Funke
;*              
;* DESCRIPTION  : turn external memory on
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

XRAM_On:
	push    temp1
	in      temp1, MCUCR
	sbr     temp1, (1<<SRE)		;set SRE                
	out     MCUCR, temp1		;enable external memory 
	pop     temp1
	ret	
       
;***********************************************************************
;* FUNCTION	: XRAM_Off 
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 13.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : turn external memory off
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

XRAM_Off:
	push    temp1
	in      temp1, MCUCR
	cbr     temp1, (1<<SRE)		;clear SRE              
	out     MCUCR, temp1		;disable external memory        
	pop     temp1
	ret	

;***********************************************************************
;* FUNCTION	: XRAM_WaitStateOn
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 13.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : turn wait state on
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

XRAM_WaitStateOn:
	push    temp1
	in      temp1, MCUCR
	sbr     temp1, (1<<SRW)		;set SRW
	out     MCUCR, temp1		;enable wait state
	pop     temp1
	ret	

;***********************************************************************
;* FUNCTION	: XRAM_WaitStateOff
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 13.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : turn wait state off
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

XRAM_WaitStateOff:
	push    temp1
	in      temp1, MCUCR
	cbr     temp1, (1<<SRW)		;clear SRW
	out     MCUCR, temp1		;disable wait state
	pop     temp1
	ret	

;***********************************************************************
	endif

;*
;* $Log: xram.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
