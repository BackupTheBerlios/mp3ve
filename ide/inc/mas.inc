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
;* $Id: mas.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : MAS Include-File
;*  Version         : 1.5
;*
;*  Filename        : mas.inc
;*  Created         : 04.05.2000 by Marc Dressler
;*  Last changed    : 07.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : basic mas functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************

	ifndef	masinc	; prevent multiple binding

masinc	equ	1

		if	mompass=1
                 message "MAS Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************
	
	INCLUDE "mp3.inc"

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

;MAS_I2CAdr	EQU	= 0bxxxxxxxx

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: MAS_Init
;*
;* CREATED	: 04.05.2000 by Marc Dressler
;*              
;* LAST CHANGED : 04.05.2000 by Marc Dressler
;*              
;* DESCRIPTION  : initialize MAS chip
;*
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************

MAS_Init:
	ret

;***********************************************************************
;* FUNCTION	: MAS_SetVolume
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : set volume
;*
;* INPUT        : param2 - volume
;*
;* OUTPUT       : -
;*
;***********************************************************************

MAS_SetVolume:
	ret

;***********************************************************************

	endif

;*
;* $Log: mas.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
