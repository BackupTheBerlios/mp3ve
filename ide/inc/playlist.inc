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
;* $Id: playlist.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : PlayList Include-File
;*  Version         : 1.5
;*
;*  Filename        : playlist.inc
;*  Created         : 26.12.2000 by Marc Dressler
;*  Last changed    : 31.03.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : playlist modul
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : ieeprom.inc
;*
;***********************************************************************

	ifndef	playlistinc	; prevent multiple binding

playlistinc	equ	1

		if	mompass=1
                 message "Playlist Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

        INCLUDE "ieeprom.inc"

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************


;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

	SEGMENT DATA


        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: PlayList_Init
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 27.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : init playlist system
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_Init:
	push	param1
	push	param2
	push	param3
	

	ldi	param2, hi(IEEPROM_Autostart_Flags)
	ldi	param3, lo(IEEPROM_Autostart_Flags)
	rcall	IEEProm_ReadByte

	sbrc	param1, IEEPROM_Autostart_fdisable
	rjmp	PlayList_InitEnd		; no autostart

	rcall	PlayList_Get			; get track informations

	rcall	MP3_AutoSkipOn
        rcall   MP3_Skip

        rcall   MP3_Play                        ; start playing

PlayList_InitEnd:
	pop	param3
	pop	param2
	pop	param1
	ret
	
;***********************************************************************
;* FUNCTION	: PlayList_Write
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : write trackinformations to playlist 
;*                preversion - will by changed !!
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_Write:
	push	param1
	push	param2
	push	param3
	
	lds	param1, MP3_NextSector1 
	ldi	param2, hi(IEEPROM_Autostart_Sector1)
	ldi	param3, lo(IEEPROM_Autostart_Sector1)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSector2
	ldi	param2, hi(IEEPROM_Autostart_Sector2)
	ldi	param3, lo(IEEPROM_Autostart_Sector2)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSector3
	ldi	param2, hi(IEEPROM_Autostart_Sector3)
	ldi	param3, lo(IEEPROM_Autostart_Sector3)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSector4
	ldi	param2, hi(IEEPROM_Autostart_Sector4)
	ldi	param3, lo(IEEPROM_Autostart_Sector4)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSectorCount1
	ldi	param2, hi(IEEPROM_Autostart_SectorCount1)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount1)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSectorCount2
	ldi	param2, hi(IEEPROM_Autostart_SectorCount2)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount2)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSectorCount3
	ldi	param2, hi(IEEPROM_Autostart_SectorCount3)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount3)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSectorCount4
	ldi	param2, hi(IEEPROM_Autostart_SectorCount4)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount4)
	rcall	IEEProm_WriteByte

	lds	param1, MP3_NextSource
	ldi	param2, hi(IEEPROM_Autostart_Source)
	ldi	param3, lo(IEEPROM_Autostart_Source)
	rcall	IEEProm_WriteByte

	pop	param3
	pop	param2
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: PlayList_Read
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : get trackinformation from playlist
;*                preversion - will by changed !!
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_Read:
	ret

;***********************************************************************
;* FUNCTION	: PlayList_Get
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : get track from playlist and set NextTrack
;*                preversion - will by changed !!
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_Get:
	push	param1
	push	param2
	push	param3
	
	ldi	param2, hi(IEEPROM_Autostart_Sector1)
	ldi	param3, lo(IEEPROM_Autostart_Sector1)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSector1, param1

	ldi	param2, hi(IEEPROM_Autostart_Sector2)
	ldi	param3, lo(IEEPROM_Autostart_Sector2)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSector2, param1

	ldi	param2, hi(IEEPROM_Autostart_Sector3)
	ldi	param3, lo(IEEPROM_Autostart_Sector3)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSector3, param1

	ldi	param2, hi(IEEPROM_Autostart_Sector4)
	ldi	param3, lo(IEEPROM_Autostart_Sector4)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSector4, param1

	ldi	param2, hi(IEEPROM_Autostart_SectorCount1)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount1)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSectorCount1, param1

	ldi	param2, hi(IEEPROM_Autostart_SectorCount2)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount2)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSectorCount2, param1

	ldi	param2, hi(IEEPROM_Autostart_SectorCount3)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount3)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSectorCount3, param1

	ldi	param2, hi(IEEPROM_Autostart_SectorCount4)
	ldi	param3, lo(IEEPROM_Autostart_SectorCount4)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSectorCount4, param1

	ldi	param2, hi(IEEPROM_Autostart_Source)
	ldi	param3, lo(IEEPROM_Autostart_Source)
	rcall	IEEProm_ReadByte
	sts	MP3_NextSource, param1

	pop	param3
	pop	param2
	pop	param1
	ret
	
;***********************************************************************
;* FUNCTION	: PlayList_Pos
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : 
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_Pos:
	ret
	
;***********************************************************************
;* FUNCTION	: PlayList_Forward
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : 
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_Forward:
	ret
	
;***********************************************************************
;* FUNCTION	: PlayList_Back
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : 
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_Back:
	ret

;***********************************************************************
;* FUNCTION	: PlayList_AutoStartOn
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 27.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : set autostart-flag in ieeprom
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_AutoStartOn:
	push	param1
	push	param2
	push	param3
	
	ldi	param2, hi(IEEPROM_Autostart_Flags)
	ldi	param3, lo(IEEPROM_Autostart_Flags)
	rcall	IEEProm_ReadByte

	cbr	param1, (1<<IEEPROM_Autostart_fdisable)

	rcall	IEEProm_WriteByte
	pop	param3
	pop	param2
	pop	param1
	ret
		
;***********************************************************************
;* FUNCTION	: PlayList_AutoStartOff
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 27.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : clear autostart-flag in ieeprom
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
PlayList_AutoStartOff:
	push	param1
	push	param2
	push	param3
	
	ldi	param2, hi(IEEPROM_Autostart_Flags)
	ldi	param3, lo(IEEPROM_Autostart_Flags)
	rcall	IEEProm_ReadByte

	sbr	param1, (1<<IEEPROM_Autostart_fdisable)

	rcall	IEEProm_WriteByte

	pop	param3
	pop	param2
	pop	param1
	ret
		
;***********************************************************************

	endif

;*
;* $Log: playlist.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
