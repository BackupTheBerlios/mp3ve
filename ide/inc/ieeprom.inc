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
;* $Id: ieeprom.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : Internal EEPROM Include-File
;*  Version         : 1.5
;*
;*  Filename        : ieeprom.inc
;*  Created         : 26.12.2000 by Marc Dressler
;*  Last changed    : 27.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : basic internal eeprom functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************

	ifndef	ieeprominc	; prevent multiple binding

ieeprominc	equ	1

	if	mompass=1
	  message "IEEPROM Include-File V1.5 (C)2000 Marc Dressler"
	endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

; ieeprom Memory-Map

IEEPROM_Autostart_Flags 	equ	1
IEEPROM_Autostart_Sector1 	equ	2	; LBA (LSB)
IEEPROM_Autostart_Sector2 	equ	3
IEEPROM_Autostart_Sector3 	equ	4
IEEPROM_Autostart_Sector4 	equ	5	; LBA (MSB)
IEEPROM_Autostart_SectorCount1 	equ	6	; (LSB)
IEEPROM_Autostart_SectorCount2 	equ	7
IEEPROM_Autostart_SectorCount3 	equ	8
IEEPROM_Autostart_SectorCount4 	equ	9	; (MSB)
IEEPROM_Autostart_Source 	equ	10

; Flag - bit definitions

IEEPROM_Autostart_fdisable	equ	0
IEEPROM_Autostart_f1	equ	1
IEEPROM_Autostart_f2	equ	2
IEEPROM_Autostart_f3	equ	3
IEEPROM_Autostart_f4	equ	4
IEEPROM_Autostart_f5	equ	5
IEEPROM_Autostart_f6	equ	6
IEEPROM_Autostart_f7	equ	7


;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: IEEProm_ReadByte
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : read byte from internal eeprom
;*
;* INPUT        : param2 - msb adress
;*                param3 - lsb adress 
;*
;* OUTPUT       : param1 - result data
;*
;***********************************************************************

IEEProm_ReadByte:
	sbic    EECR, EEWE
        rjmp    IEEProm_ReadByte

        out     EEARH, param2
        out     EEARL, param3

        sbi     EECR, EERE

        in	param1, EEDR
	ret

;***********************************************************************
;* FUNCTION	: IEEProm_WriteByte
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 27.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : write byte to internal eeprom
;*
;* INPUT        : param1 - data
;*                param2 - MSB adress
;*                param3 - LSB adress
;*
;* OUTPUT       : -
;*
;***********************************************************************

IEEProm_WriteByte:
        sbic    EECR, EEWE
        rjmp    IEEProm_WriteByte

        out     EEARH, param2
        out     EEARL, param3

        out     EEDR, param1

	cli
        sbi     EECR, EEMWE
        sbi     EECR, EEWE
        sei
        
	ret

;***********************************************************************

	endif

;*
;* $Log: ieeprom.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
