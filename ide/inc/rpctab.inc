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
;* $Id: rpctab.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Remote Procedure Call Command Table
;*  Version     : 1.5
;*				
;*  Filename    : rpctab.inc
;*  Created     : 19.01.2001 by Marc Dressler
;*  Last changed: 01.04.2001 by Marc Dressler
;*	
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : communcation interface for mp3ctrlio and mp3ide
;*
;*  Homepage    : www.MP3ve.de
;*                www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : rpc.inc
;*		  mp3.inc
;*		  debug.inc
;*		  tocj.inc
;*		  playlist.inc
;*
;***********************************************************************
	
	ifndef	rcptabinc		;prevent multiple binding

rcptabinc	equ	1

	if       mompass=1
	  message "RPC-Command-Table Include V1.5 (C) 2001 by Marc Dressler"
	endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "rpc.inc"
	INCLUDE "uart.inc"
	INCLUDE "mp3.inc"
	INCLUDE "debug.inc"
	INCLUDE "tocj.inc"
	INCLUDE "ieeprom.inc"
  ifndef PlayList_disable	   
	INCLUDE "playlist.inc"
  endif

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************


;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

	SEGMENT	DATA

	SEGMENT CODE				     

	; RPC CommandID Table
RPC_CMD_SysReset:		equ	0
RPC_CMD_Stop:			equ	1
RPC_CMD_Play:			equ	2
RPC_CMD_Set_CurrentTrack:	equ	3
RPC_CMD_TOC_Read:		equ	4
RPC_CMD_DEV_SelectMaster:	equ	5
RPC_CMD_DEV_SelectSlave:	equ	6
RPC_CMD_AutoSkip:		equ	7
RPC_CMD_SelectSource:		equ	8
RPC_CMD_Pause:			equ	9
RPC_CMD_Debug_Info:		equ	10
RPC_CMD_Skip:			equ	11
RPC_CMD_Set_NextTrack:		equ	12
RPC_CMD_Get_CurrentTrack:	equ	13
RPC_CMD_Set_Volume:		equ	14
RPC_CMD_MP3_SetSpeed:		equ	15
RPC_CMD_Ping:			equ	16
RPC_CMD_Nop:			equ	17
RPC_CMD_PlayList_Write:		equ	18
RPC_CMD_PlayList_Pos:		equ	19
RPC_CMD_PlayList_Forward:	equ	20
RPC_CMD_PlayList_Back:		equ	21
RPC_CMD_PlayList_AutoStart:	equ	22
RPC_CMD_free23:		equ	23 ;   free
RPC_CMD_EEPROM_Write:		equ	24
RPC_CMD_EEPROM_Read:		equ	25
RPC_CMD_SYS_Info:		equ	26
RPC_CMD_DEV_Copy:		equ	27
RPC_CMD_DEV_ReadSector:		equ	28
RPC_CMD_DEV_WriteSector:	equ	29
RPC_CMD_DEV_GetDeviceInfo:	equ	30
RPC_CMD_MP3_FastForward:	equ	31
RPC_CMD_MP3_FastBackward:	equ	32
RPC_CMD_I2C_Write:		equ	33
RPC_CMD_I2C_Read:		equ	34
RPC_CMD_PlayList_Read:		equ	35
RPC_CMD_Mem_Write:		equ	36
RPC_CMD_Mem_Read:		equ	37
	
;***********************************************************************
;* FUNCTION	: RPC_Commands
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Command jump table
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Commands:					;* - not implemented yet
	data	RPC_SYS_Reset			; 0x00(000) System Reset
	data	RPC_Stop			; 0x01(001) Stop playing
	data	RPC_Play			; 0x02(002) Start playing
	data	RPC_Set_CurrentTrack		; 0x03(003) CurrentTrack = RPC-Param (Old name : RPC_MP3_Next)
	data	RPC_TOC_Read			; 0x04(004) Read Toc from activ device
	data	RPC_DEV_SelectMaster		; 0x05(005) <delete>
	data	RPC_DEV_SelectSlave		; 0x06(006) <delete>
	data	RPC_AutoSkip			; 0x07(007)	
	data	RPC_SelectSource		; 0x08(008) Select Master / Slave / Slot / Audio / MP3 / ...
	data	RPC_Pause			; 0x09(009)
	data	RPC_Debug_Info			; 0x0a(010)
	data	RPC_Skip			; 0x0b(011) CurrentTrack = NextTrack
	data	RPC_Set_NextTrack		; 0x0c(012) NextTrack = RPC-Param	
	data	RPC_Get_CurrentTrack		; 0x0d(013) Send CurrentTrack to uart
	data	RPC_Set_Volume			;!0x0e(014) sta implemented, mas will come.. 
	data	RPC_MP3_SetSpeed		;*0x0f(015) 
	data	RPC_Ping			; 0x10(016) answer with first received param
	data	RPC_Nop				; 0x11(017) do nothing

	data	RPC_PlayList_Write		; 0x12(018) write trackinformation to playlist
	data	RPC_PlayList_Pos		;*0x13(019) NextTrack = track x in playlist
	data	RPC_PlayList_Forward		;*0x14(020) NextTrack = next track in playlist
	data	RPC_PlayList_Back		;*0x15(021) NextTrack = track back in playlist
	data	RPC_PlayList_AutoStart		; 0x16(022) enable/disable autoplay after power on
	data	RPC_free			; 0x17(023) <delete> free
	
	data	RPC_EEPROM_Write		; 0x18(024) write data to internal eeprom
	data	RPC_EEPROM_Read			; 0x19(025) read data from internal eeprom
	
	data	RPC_SYS_Info			; 0x1a(026) get system informations

	data	RPC_DEV_Copy			; 0x1b(027) copy sectors from device to device
	data	RPC_DEV_ReadSector		;*0x1c(028) read sector and copy to uart
	data	RPC_DEV_WriteSector		;*0x1d(029) read data from uart and write to device
	data	RPC_DEV_GetDeviceInfo		; 0x1e(030) get information from devices

	data	RPC_MP3_FastForward		;*0x1f(031) play fast forward
	data	RPC_MP3_FastBackward		;*0x20(032) play fast backward

	data	RPC_I2C_Write			;*0x21(033) write data to i2c bus 
	data	RPC_I2C_Read			;*0x22(034) read data from i2c bus

	data	RPC_PlayList_Read		;*0x23(035) read track-informations from playlist

	data	RPC_Mem_Write			;*0x22(036) write data to memory
	data	RPC_Mem_Read			;*0x23(037) read data from memory

RPC_CommandsEnd:

;***********************************************************************
;* FUNCTION	: RPC_I2C_Write
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_I2C_Write:
	ret
	
;***********************************************************************
;* FUNCTION	: RPC_I2C_Read
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_I2C_Read:
	ret
	
;***********************************************************************
;* FUNCTION	: RPC_Play
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Play:
	rjmp	MP3_Play

;***********************************************************************
;* FUNCTION	: RPC_Stop
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Stop:
	rjmp	MP3_Stop

;***********************************************************************
;* FUNCTION	: RPC_Pause
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Pause:
	rjmp	MP3_Pause

;***********************************************************************
;* FUNCTION	: RPC_Skip
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Skip:
	rjmp	MP3_Skip

;***********************************************************************
;* FUNCTION	: RPC_Set_NextTrack
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Set_NextTrack:
	ldi	ZL, lo(RPC_RxData)
	ldi	ZH, hi(RPC_RxData)

	ld	temp1, Z+
	sts	MP3_NextSector4, temp1	; LBA (MSB)
	ld	temp1, Z+
	sts	MP3_NextSector3, temp1
	ld	temp1, Z+
	sts	MP3_NextSector2, temp1
	ld	temp1, Z+
	sts	MP3_NextSector1, temp1	; LBA (LSB)

	ld	temp1, Z+
	sts	MP3_NextSectorCount4, temp1	; Count (MSB)
	ld	temp1, Z+
	sts	MP3_NextSectorCount3, temp1
	ld	temp1, Z+
	sts	MP3_NextSectorCount2, temp1
	ld	temp1, Z+
	sts	MP3_NextSectorCount1, temp1	

	ld	temp1, Z+
	sts	MP3_NextSource, temp1	

	ret

;***********************************************************************
;* FUNCTION	: RPC_Set_CurrentTrack
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Set_CurrentTrack:

	ldi	ZL, lo(RPC_RxData)
	ldi	ZH, hi(RPC_RxData)
	
	ld	temp1, Z+
	sts	MP3_Sector4, temp1	; LBA (MSB)
	ld	temp1, Z+
	sts	MP3_Sector3, temp1
	ld	temp1, Z+
	sts	MP3_Sector2, temp1
	ld	temp1, Z+
	sts	MP3_Sector1, temp1	; LBA (LSB)

	ld	temp1, Z+
	sts	MP3_SectorCount4, temp1	; Count (MSB)
	ld	temp1, Z+
	sts	MP3_SectorCount3, temp1
	ld	temp1, Z+
	sts	MP3_SectorCount2, temp1
	ld	temp1, Z+
	sts	MP3_SectorCount1, temp1	

	ld	temp1, Z+
	sts	MP3_Source, temp1	

	ret

	
;***********************************************************************
;* FUNCTION	: RPC_DEV_SelectSlave
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_DEV_SelectSlave:
	rcall	MP3_Stop
	rjmp	DEV_Select_Slave_CMD

;***********************************************************************
;* FUNCTION	: RPC_DEV_SelectMaster
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_DEV_SelectMaster:
	rcall	MP3_Stop
	rjmp	DEV_Select_Master_CMD
		
;***********************************************************************
;* FUNCTION	: RPC_TOC_Read
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_TOC_Read:
	rcall	MP3_Stop
	rcall	TOC_Read
	rjmp	MP3_ReInit

;***********************************************************************
;* FUNCTION	: RPC_AutoSkip
;*
;* CREATED	: 01.04.2001 by Marc Dressler
;*
;* LAST CHANGED : 01.04.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*                RPC_RxData0 - 0x00 off
;*                            - 0x01 on	
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_AutoSkip:
	lds	param1, RPC_RxData0
	cpi	param1, 0x00	; off
	breq	RPC_AutoSkip_Off
RPC_AutoSkip_On:
	rjmp	MP3_AutoSkipOn
RPC_AutoSkip_Off:
	rjmp	MP3_AutoSkipOff

;***********************************************************************
;* FUNCTION	: RPC_Nop
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************
RPC_free:	
RPC_Nop:
	ret

;***********************************************************************
;* FUNCTION	: RPC_SYS_Reset
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_SYS_Reset:	
	rjmp	reset
	
;***********************************************************************
;* FUNCTION	: RPC_SYS_Info
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_SYS_Info:	
        ldi     zl,lo(VersionPlain*2)
        ldi     zh,hi(VersionPlain*2)
        rjmp	UART_WriteTxt
	
;***********************************************************************
;* FUNCTION	: RPC_Debug_Info
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Debug_Info:
	rjmp	Debug_Info

;***********************************************************************
;* FUNCTION	: RPC_Get_CurrentTrack
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Get_CurrentTrack:
	rjmp	MP3_GetCurrentTrack
	
;***********************************************************************
;* FUNCTION	: RPC_Set_Volume
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Set_Volume:
	lds	param2, RPC_RxData0
	rjmp	MP3_SetVolume
	
;***********************************************************************
;* FUNCTION	: RPC_MP3_SetSpeed
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_MP3_SetSpeed:
	lds	param1, RPC_RxData0	
	rjmp	MP3_SetSpeed
	
;***********************************************************************
;* FUNCTION	: RPC_PlayList_Write
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_PlayList_Write:
  ifndef PlayList_disable
	rjmp	PlayList_Write
  endif
  ifdef PlayList_disable
	ret
  endif
	
;***********************************************************************
;* FUNCTION	: RPC_PlayList_Read
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_PlayList_Read:
  ifndef PlayList_disable	
	rjmp	PlayList_Read
  endif
  ifdef PlayList_disable
	ret
  endif
	
;***********************************************************************
;* FUNCTION	: RPC_
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_PlayList_Pos:
  ifndef PlayList_disable
	rjmp	PlayList_Pos
  endif
  ifdef PlayList_disable
	ret
  endif
	
;***********************************************************************
;* FUNCTION	: RPC_
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_PlayList_Forward:
  ifndef PlayList_disable
	rjmp	PlayList_Forward
  endif
  ifdef PlayList_disable
	ret
  endif
	
;***********************************************************************
;* FUNCTION	: RPC_
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_PlayList_Back:
  ifndef PlayList_disable
	rjmp	PlayList_Back
  endif
  ifdef PlayList_disable
	ret
  endif
	
;***********************************************************************
;* FUNCTION	: RPC_PlayList_AutoStartOn
;*
;* CREATED	: 01.04.2001 by Marc Dressler
;*
;* LAST CHANGED : 01.04.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_PlayList_AutoStart:
  ifndef PlayList_disable
	lds	param1, RPC_RxData0
	cpi	param1, 0x00	; off
	breq	RPC_AutoSkip_Off
RPC_PlayList_AutoStart_On:
	rjmp	PlayList_AutoStartOn
RPC_PlayList_AutoStart_Off:
	rjmp	PlayList_AutoStartOff
  endif
  ifdef PlayList_disable
	ret
  endif

;***********************************************************************
;* FUNCTION	: RPC_EEPROM_Write
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_EEPROM_Write:
	lds	param2, RPC_RxData0	; high byte eeprom adr.
	lds	param3, RPC_RxData1	; low  byte eeprom adr.
	lds	param1, RPC_RxData2	; data-byte
	rcall	IEEProm_WriteByte

	rcall	IEEProm_ReadByte	; read for verify

	sts	RPC_TxData0, param2	; high byte eeprom adr.
	sts	RPC_TxData1, param3	; low  byte eeprom adr.
	sts	RPC_TxData2, param1	; data-byte

	ldi	param1, RPC_CMD_EEPROM_Write
	sts	RPC_TxID, param1

	ldi	param1, 3
	sts	RPC_TxLen, param1 	; return 3 bytes (RPC_TxData0..2) 

	rjmp	RPC_Transmit

;***********************************************************************
;* FUNCTION	: RPC_EEPROM_Read
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_EEPROM_Read:
	lds	param2, RPC_RxData0	; high byte eeprom adr.
	lds	param3, RPC_RxData1	; low  byte eeprom adr.
	rcall	IEEProm_ReadByte

	sts	RPC_TxData0, param2	; high byte eeprom adr.
	sts	RPC_TxData1, param3	; low  byte eeprom adr.
	sts	RPC_TxData2, param1	; data-byte

	ldi	param1, RPC_CMD_EEPROM_Read
	sts	RPC_TxID, param1

	ldi	param1, 3
	sts	RPC_TxLen, param1 	; return 3 bytea (RPC_TxData0..2) 

	rjmp	RPC_Transmit

;***********************************************************************
;* FUNCTION	: RPC_Dev_Copy
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************
	
RPC_Dev_Copy:	
  ifndef DevCopy_disabled

	rcall	MP3_Stop

	ldi	ZL, lo(RPC_RxData)
	ldi	ZH, hi(RPC_RxData)
	
	ld	param1, Z+
	sts	DEV_Source_Device, param1	; Bit 4=0 Master 
	ld	param1, Z+
	sts	DEV_Dest_Device, param1		; Bit 4=0 Master 

	ld	param1, Z+	
	sts	DEV_Source_StartSector3, param1	; (MSB)
	ld	param1, Z+	
	sts	DEV_Source_StartSector2, param1
	ld	param1, Z+	
	sts	DEV_Source_StartSector1, param1
	ld	param1, Z+	
	sts	DEV_Source_StartSector0, param1	; (LSB) 

	ld	param1, Z+	
	sts	DEV_Source_Len3, param1	; (MSB) 
	ld	param1, Z+	
	sts	DEV_Source_Len2, param1
	ld	param1, Z+	
	sts	DEV_Source_Len1, param1
	ld	param1, Z+	
	sts	DEV_Source_Len0, param1	; (LSB) 

	ld	param1, Z+	
	sts	DEV_Dest_StartSector3, param1	; (MSB)
	ld	param1, Z+	
	sts	DEV_Dest_StartSector2, param1
	ld	param1, Z+	
	sts	DEV_Dest_StartSector1, param1
	ld	param1, Z+	
	sts	DEV_Dest_StartSector0, param1	; (LSB) 
	
	ldi	param1, RPC_CMD_Dev_Copy
	sts	RPC_TxID, param1

	ldi	param1, 1		; 1 - start copy-process
	sts	RPC_TxData0, param1

	ldi	param1, 1
	sts	RPC_TxLen, param1 	; 1 return byte (RPC_TxData0) 

	rcall	RPC_Transmit

	rcall	DEV_Copy_CMD

	ldi	param1, 0		; 0 - end copy-process without errors
	sts	RPC_TxData0, param1
	rjmp	RPC_Transmit
  endif
	
  ifdef DevCopy_disabled
	ret
  endif
	
;***********************************************************************
;* FUNCTION	: RPC_DEV_ReadSector
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_DEV_ReadSector:
	rcall	MP3_Stop
	ret

;***********************************************************************
;* FUNCTION	: RPC_DEV_WriteSector
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_DEV_WriteSector:
	rcall	MP3_Stop
	ret

;***********************************************************************
;* FUNCTION	: RPC_DEV_GetDeviceInfo
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_DEV_GetDeviceInfo:
	lds	param1, DEV_StatusFlags	; Master/Slave present, ATA or ATAPI, selected Device
	rcall	UART_Write
	ret

;***********************************************************************
;* FUNCTION	: RPC_MP3_FastForward
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_MP3_FastForward:
	ret

;***********************************************************************
;* FUNCTION	: RPC_MP3_FastBackward
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_MP3_FastBackward:
	ret

;***********************************************************************
;* FUNCTION	: RPC_
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Ping:
  ifdef rpc_old
	lds	param1, RPC_RxData0
	rjmp	UART_Write
  endif
  ifndef rpc_old
	ldi	param1, RPC_CMD_Ping
	sts	RPC_TxID, param1

	lds	param1, RPC_RxData0
	sts	RPC_TxData0, param1

	ldi	param1, 1
	sts	RPC_TxLen, param1 	; 1 return byte (RPC_TxData0) 

	rjmp	RPC_Transmit
  endif


;***********************************************************************
;* FUNCTION	: RPC_EEPROM_Write
;*
;* CREATED	: 31.03.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Mem_Write:
	;; RPC_RxData0 - length
	lds	zh, RPC_RxData1		; high byte adr.
	lds	zl, RPC_RxData2		; low byte adr.
	lds	param1, RPC_RxData3	; data-byte
	st	z, param1
	ret

;***********************************************************************
;* FUNCTION	: RPC_SelectSource
;*
;* CREATED	: 01.04.2001 by Marc Dressler
;*
;* LAST CHANGED : 01.04.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_SelectSource:
	lds	param1, RPC_RxData0	; see MP3_Source structure
	rjmp	MP3_SelectSource

;***********************************************************************
;* FUNCTION	: RPC_EEPROM_Read
;*
;* CREATED	: 31.03.2001 by Marc Dressler
;*
;* LAST CHANGED : 31.03.2001 by Marc Dressler
;*
;* DESCRIPTION  : RPC Commands
;*
;* INPUT        : RPC data frame
;*
;* OUTPUT       : -
;*
;***********************************************************************

RPC_Mem_Read:
	;; RPC_RxData0 - length
	lds	zh, RPC_RxData1		; high byte adr.
	lds	zl, RPC_RxData2		; low byte adr.
	
	sts	RPC_TxData1, zh		; high byte adr.
	sts	RPC_TxData2, zl		; low  byte adr.
	
	ld	param1, z		; data-byte
	sts	RPC_TxData3, param1	; data-byte

	ldi	param1, RPC_CMD_Mem_Read
	sts	RPC_TxID, param1

	ldi	param1, 4
	sts	RPC_TxLen, param1 	; return 3 bytea (RPC_TxData0..3) 

	rjmp	RPC_Transmit
		
;***********************************************************************

	endif

;*
;* $Log: rpctab.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
