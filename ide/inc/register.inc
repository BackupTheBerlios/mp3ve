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
;* $Id: register.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Register
;*  Version     : 1.5
;*
;*  Filename    : register.inc
;*  Created     : 12.12.1999 by Marc Dressler
;*  Last changed: 29.12.2000 by Marc Dressler
;*
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : Registerdefinitions
;*
;*  Homepage    : www.MP3ve.de
;*                www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : -
;*
;***********************************************************************

	save
	listing off

	ifndef	registerinc	; prevent multiple binding

registerinc	equ	1

        if       mompass=1
          message "Register Definitions V1.5 (C)1999 by Marc Dressler"
        endif

;***********************************************************************
;* GLOBAL DEFINITIONS
;***********************************************************************

reg0		reg	r0	;reserved for spezial commands (LPM,..)

temp5		reg	r1     	;SBCI, SUBI, CPI, ANDI, ORI are not supported
temp6		reg	r2
temp7		reg	r3
temp8		reg	r4

param5		reg	r5     	;SBCI, SUBI, CPI, ANDI, ORI are not supported
param6		reg	r6
param7		reg	r7
param8		reg	r8

param4		reg	r9	;parameter for subroutine
;	reg	r10
MP3_Flags1	reg	r11

MP3_TailLo	reg	r12	;only for MP3-Queue: must be reg_hi-1 (even)
MP3_TailHi	reg	r13	;only for MP3-Queue: must be reg_lo+1 (odd)
MP3_SREG	reg	r14	;only for MP3-Queue
MP3_Data	reg	r15	;only for MP3-Queue

temp1		reg	r16	;for free use
temp2		reg	r17	;for free use
temp3		reg	r18	;for free use
temp4		reg	r19	;for free use

param1		reg	r20	;parameter for subroutine
param2		reg	r21	;parameter for subroutine
param3		reg 	r22	;parameter for subroutine
MP3_Flags	reg	r23	;only for MP3-Queue

tmpL    	reg     r24    	;ADIW and SBIW will only operates with r24 and r25
tmpH    	reg     r25    	;(r24 - r31, but r26-r31 are still used)

XL      	reg	r26	;X-Register used for indirect addressing
XH      	reg     r27
YL      	reg     r28	;Y-Register used for MP3
YH      	reg     r29
ZL      	reg     r30	;Z-Register used for indirect addressing
ZH      	reg     r31

MP3_HeadLo	reg	r28	;only for MP3-Queue: must be reg_hi-1 (even)
MP3_HeadHi	reg	r29	;only for MP3-Queue: must be reg_lo+1 (odd)

;***********************************************************************

	endif

	listing on
	restore

;*
;* $Log: register.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
