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
;* $Id: delay.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : Delay Include-File
;*  Version         : 1.5
;*
;*  Filename        : delay.inc
;*  Created         : 06.11.1999 by Marc Dressler
;*  Last changed    : 14.01.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : all
;*  Description     : preversion
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************
 
	ifndef	delayinc	; prevent multiple binding

delayinc	equ	1

		if	mompass=1
                 message "Delay Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************


;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: Delay1us
;*
;* CREATED	: 26.04.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.04.2000 by Marc Dressler
;*
;* DESCRIPTION  : wait for 1us
;*                ATTENTION : for 8MHz systemclock
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Delay1us:                       ;8 cycles = 1us
                                ;3 cycles = rcall
        nop                     ;1 cyles
        ret                     ;4 cycles

;***********************************************************************
;* FUNCTION	: Delay5us
;*
;* CREATED	: 26.04.2000 by Marc Dressler
;*
;* LAST CHANGED : 14.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : wait for 5us
;*                ATTENTION : for 8MHz systemclock
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Delay5us:
        push    temp1
                                ;40 cycles = 5us
                                ;3 cycles = rcall
        ldi     temp1,12        ;1 cycle
Delay5uL:
        dec     temp1           ;1 cyles
        brne    Delay5uL        ;jump: 2 cyles / no jump: 1 cyles
        pop     temp1
        ret                     ;4 cycles

;***********************************************************************
;* FUNCTION	: Delay10us
;*
;* CREATED	: 26.04.2000 by Marc Dressler
;*
;* LAST CHANGED : 14.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : wait for 10us
;*                ATTENTION : for 8MHz systemclock
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Delay10us:
        push    temp1
                                ;80 cycles = 10us
                                ;3 cycles = rcall
        ldi     temp1,24        ;1 cycle
Delay10uL:
        dec     temp1           ;1 cyles
        brne    Delay10uL       ;jump: 2 cyles / no jump: 1 cyles
        pop     temp1
        ret                     ;4 cycles


;***********************************************************************
;* FUNCTION	: Delay100us
;*
;* CREATED	: 26.04.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.04.2000 by Marc Dressler
;*
;* DESCRIPTION  : wait for 100us
;*                ATTENTION : for 8MHz systemclock
;*		!!!!!!! NOT READY !!!!!!!!!
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Delay100us:                     ;800 cycles = 100us
        ;3 cycles = rcall
        push	temp2
        ldi     temp2,97        ;1 cycle
Delay100uL:
        rcall   Delay1us        ;8 cyles
        dec     temp2           ;1 cyles
        brne    Delay100uL      ;jump: 2 cyles / no jump: 1 cyles
        pop	temp2
        ret                     ;4 cycles


;***********************************************************************
;* FUNCTION	: Delay1ms
;*
;* CREATED	: 26.04.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.04.2000 by Marc Dressler
;*
;* DESCRIPTION  : wait for 1ms
;*                ATTENTION : for 8MHz systemclock
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Delay1ms:                       ;8000 cycles = 1ms
                                ;3 cycles = rcall
        push	temp3
        ldi     temp3,9         ;1 cycle
Delay1mL:
        rcall   Delay100us      ;800 cyles
        dec     temp3           ;1 cyles
        brne    Delay1mL        ;jump: 2 cyles / no jump: 1 cyles
	pop	temp3
        ret                     ;4 cycles


;***********************************************************************
;* FUNCTION	: Delay10ms
;*
;* CREATED	: 26.04.2000 by Marc Dressler
;*
;* LAST CHANGED : 14.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : wait for 10ms
;*                ATTENTION : for 8MHz systemclock
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Delay10ms:                      ;80.000 cycles = 10ms
                                ;3 cycles = rcall
	push	temp1
	push	temp2
	
        ldi     temp1,105       ;1 cycle
Delay10mL1:
        ldi     temp2,0xff      ;1 cycle
Delay10mL2:
        dec     temp2           ;1 cyles
        brne    Delay10mL2      ;jump: 2 cyles / no jump: 1 cyles
        dec     temp1           ;1 cyles
        brne    Delay10mL1      ;jump: 2 cyles / no jump: 1 cyles

	pop	temp2
	pop	temp1
        ret                     ;4 cycles


;***********************************************************************
;* FUNCTION	: Delay
;*
;* CREATED	: 26.04.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.04.2000 by Marc Dressler
;*
;* DESCRIPTION  : wait for a long unknown time
;*		!!!!!!! NOT READY !!!!!!!!!
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

Delay:
        push    temp1
        push    temp2
        push    temp3


        ldi     temp1,$40
WL1:
        ldi     temp2,$ff
WL2:
        ldi     temp3,$ff
WL3:
        dec     temp3
        brne    WL3

        dec     temp2
        brne    WL2

        dec     temp1
        brne    WL1

        pop     temp3
        pop     temp2
        pop     temp1
        ret

;***********************************************************************

	endif

;*
;* $Log: delay.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
