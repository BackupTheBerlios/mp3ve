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
;* $Id: debugdef.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : Debug Include-File
;*  Version         : 1.5
;*
;*  Filename        : debug.inc
;*  Created         : 21.01.2001 by Marc Dressler
;*  Last changed    : 21.01.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : debug modul definitions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************

	ifndef	debugdefinc	; prevent multiple binding

debugdefinc	equ	1

		if	mompass=1
                 message "Debug Definitions Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

; do not change file 

;DebugID_Sys_:			equ	0x0000	; 0x00?? for system

DebugID_Sys_Nop:		equ	0x0000	; nothing
DebugID_Sys_Reset:		equ	0x0001	; system reset
DebugID_Sys_Forever:		equ	0x0002	; forever loop reached

;DebugID_MP3_:			equ	0x0100	; 0x01?? for mp3
;DebugID_DEV_:			equ	0x0200	; 0x02?? for ata
;DebugID_ATA_:			equ	0x0300	; 0x03?? for ata
;DebugID_ATAPI_:		equ	0x0400	; 0x04?? for atapi
;DebugID_TOC_:			equ	0x0500	; 0x05?? for toc


;***********************************************************************

	endif

;*
;* $Log: debugdef.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
