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
;* $Id: atadef.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : ATA Memory-Address
;*  Version         : 1.5
;*
;*  Filename        : atadef.inc
;*  Created         : 29.11.1999 by Marc Dressler
;*  Last changed    : 23.12.2000 by Jens Funke
;*
;*  Target          : Atmel AVR AT90Sxxxx
;*  Description     : -
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************
   	ifndef  atadefinc       ; prevent multiple binding
atadefinc       equ     1

		save
                ;listing off	

                if       mompass=1
                 message "ATA Memory-Address V1.5 (C) 2000 by Jens Funke"
                endif

;***********************************************************************
;* ATA-Interface Adresses
;***********************************************************************
; /CS1Fx aktiv
ATA_RWADataLow  EQU       0b1100000000000000      ;1f0    RW: Data LOW IDE
ATA_WAFeature   EQU       0b1100000000000001      ;1f1    W: Feature Reg.
ATA_RAError     EQU       0b1100000000000001      ;1f1    R: Error Reg.
ATA_RWASecCount EQU       0b1100000000000010      ;1f2:   RW: SectorCount Reg. 
ATA_RWASecNo    EQU       0b1100000000000011      ;1f3:   RW: Sectornumber Reg.
ATA_RWACylLow   EQU       0b1100000000000100      ;1f4:   RW: Cylinder-low Reg.
ATA_RWACylHigh  EQU       0b1100000000000101      ;1f5:   RW: Cylinder-high Reg.
;ATA_RWABlockAdr EQU       0b1100000000000110      ;1f6:   RW: Blockaddress Reg.
ATA_RWADevHead	EQU       0b1100000000000110      ;1f6:   RW: Device/Head Reg.
ATA_WACommand   EQU       0b1100000000000111      ;1f7:   W: Command
ATA_RAStatus    EQU       0b1100000000000111      ;1f7:   R: Status

; /CS3Fx aktiv
ATA_RWADataHigh EQU       0b1100000000001000      ;3f0    RW: Data HIGH Latch
;ATA_3f1Adr      EQU       0b1100000000001001      ;3f1:
;ATA_3f2Adr      EQU       0b1100000000001010      ;3f2:
;ATA_3f3Adr      EQU       0b1100000000001011      ;3f3:
;ATA_3f4Adr      EQU       0b1100000000001100      ;3f4:
;ATA_3f5Adr      EQU       0b1100000000001101      ;3f5:
ATA_WAContol    EQU       0b1100000000001110      ;3f6:   W: Control Reg.
ATA_RAAltStatus EQU       0b1100000000001110      ;3f6:   W: Alt. Status Reg.
ATA_RAAddress   EQU       0b1100000000001111      ;3f7:   R: Address Reg. (ATA-1)


;***********************************************************************
;* IDE Commands
;***********************************************************************
ATA_AckMediaCh          EQU       0xDB             ;Acknowleged media Change
ATA_PostBoot            EQU       0xDC             ;Boot - Post-Boot
ATA_PreBoot             EQU       0xDD             ;Boot - Pre-Boot
ATA_CheckPWRMode        EQU       0xE5  ;0x98      ;Check Power Mode (E5h oder 98h)
ATA_CFAEraseSec         EQU       0xC0             ;CFA Erase Sectors
ATA_CFAReqExtErr        EQU       0x03             ;CFA Request Extended Error
ATA_CFAWriteMultW_OErase EQU       0xCD             ;CFA Write Multiple W/O Erase
ATA_CFAWriteSecW_OErase EQU       0x38             ;CFA Write Sectors W/O Erase
ATA_DevReset            EQU       0x08             ;Device Reset
ATA_DownloadMc          EQU       0x92             ;Download Microcode
ATA_ExecDevDiagnostic   EQU       0x90             ;Execute Device Diagnostic
ATA_FlushCache          EQU       0xE7             ;Flush Cache
ATA_FormatTrack         EQU       0x50             ;Format Track
ATA_GetMediaStat        EQU       0xDA             ;Get Media Status
ATA_IDDev               EQU       0xEC             ;Identify Device
ATA_IDDevDMA            EQU       0xEE             ;Identify Device DMA
ATA_IDPackDev           EQU       0xA1             ;Identify Packet Device
ATA_Idle                EQU       0x97  ;0x97/0xE3 ;Idle
ATA_IdleIm              EQU       0x91  ;0x91/0xE1 ;Idle Immediate
ATA_InitDevParam        EQU       0x91             ;Initialize Device Parameters
ATA_MediaEject          EQU       0xED             ;Media Eject
ATA_MediaLock           EQU       0xDE             ;Media Lock
ATA_MediaUnlock         EQU       0xDF             ;Media Unlock
ATA_NOP                 EQU       0x00             ;No Operation
ATA_Packet              EQU       0xA0             ;Packet
ATA_ReadBuffer          EQU       0xE4             ;Read Buffer
ATA_ReadDMALoop         EQU       0xC8             ;Read DMA with retry
ATA_ReadDMA             EQU       0xC9             ;Read DMA without retry
ATA_ReadDMAQ            EQU       0xC7             ;Read DMA Queued
ATA_DriveState          EQU       0xE9             ;Read Drive State
ATA_ReadLongLoop        EQU       0x22             ;Read Long with retry
ATA_ReadLong            EQU       0x23             ;Read Long without retry
ATA_ReadMulti           EQU       0xC4             ;Read Multiple
ATA_ReadNatMaxAdr       EQU       0xF8             ;Read Native Max Address
ATA_ReadSecLoop         EQU       0x20             ;Read Sectors with retry
ATA_ReadSec             EQU       0x21             ;Read Sectors without retry
ATA_ReadVerSecLoop      EQU       0x40             ;Read Verify Sectors with retry
ATA_ReadVerSec          EQU       0x41             ;Read Verify Sectors without retry
ATA_Recalibrate         EQU       0x10   ;0x1x     ;Recalibrate
ATA_Rest                EQU       0xE7             ;Rest
ATA_RestoreDrvState     EQU       0xEA             ;Restore Drive State
ATA_SecurDisablePwd     EQU       0xF6             ;Security Disable Password
ATA_SecurErasePrePare   EQU       0xF3             ;Security Erase Pre-Pare
ATA_SecurEraseUnit      EQU       0xF4             ;Security Erase Unit
ATA_SecurFreeze         EQU       0xF5             ;Security Freeze
ATA_SecurSetPwd         EQU       0xF1             ;Security Set Password
ATA_ScurUnlock          EQU       0xF2             ;Security Unlock
ATA_Seek                EQU       0x70   ;0x7x     ;Seek
ATA_Service             EQU       0xA2             ;Service
ATA_SetFeature          EQU       0xEF             ;Set Feature
ATA_SetMAAdr            EQU       0xF9             ;Set MA Address
ATA_SetMultiMode        EQU       0xC6             ;Set Multiple Mode
ATA_Sleep               EQU       0xE6   ;0x99     ;Sleep
ATA_Smart               EQU       0xB0             ;Smart...(6Commands)
ATA_Standby             EQU       0xE2   ;0x96     ;Standby
ATA_StandbyImm          EQU       0xE0   ;0x94     ;Standby Immediate
ATA_WriteBuffer         EQU       0xE8             ;Write Buffer
ATA_WriteDMALoop        EQU       0xCA             ;Write DMA with retry
ATA_WriteDMA            EQU       0xCB             ;Write DMA without retry
ATA_WriteDMAQ           EQU       0xCC             ;Write DMA Queued
ATA_WriteLongLoop       EQU       0x32             ;Write Long with retry
ATA_WriteLong           EQU       0x33             ;Write Long without retry
ATA_WriteMulti          EQU       0xC5             ;Write Multiple
ATA_WriteSame           EQU       0xE9             ;Write Same
ATA_WriteSecLoop        EQU       0x30             ;Write Sectors with retry
ATA_WriteSec            EQU       0x31             ;Wrtie Sectors without retry
ATA_WriteVerify         EQU       0x3C             ;Write Verify

;***********************************************************************
;* Bit Definitions
;***********************************************************************
; Error Register
ATA_MED		EQU	0
ATA_ABRT	EQU	2
ATA_IDNF	EQU	4
	
;Device Control Register
ATA_nIEN	EQU	1
ATA_SRST	EQU	2
	
;Status Register
ATA_ERR		EQU	0
ATA_DRQ		EQU	3
ATA_DF		EQU	5
ATA_DRDY	EQU	6
ATA_BSY 	EQU	7


;***********************************************************************
;* Global ATA-Variables
;***********************************************************************
	SEGMENT DATA

;***********************************************************************
;* Ticks & Source for Timeout Definitions
;***********************************************************************
; To calculate XXX_Time_XXX use:	
; XXX_Time_XXX = Time(sec) * Clk(Hz) / prescaling source
; use TIMER1_CK,TIMER1_CK_8, TIMER1_CK_64, TIMER1_CK_256 or TIMER1_CK_1024
; for prescaling source	  
;***********************************************************************
ATA_Time_NBSY		=	62500		; set CompareA to 8 sec. delay
ATA_Source_NBSY		=	TIMER1_CK_1024	;256	; prescaling source for NBSY
ATA_Time_INTRQ		=	62500		; set CompareA to 8 sec. delay
ATA_Source_INTRQ	=	TIMER1_CK_1024	; prescaling source for INTRQ
ATA_Time_DRDY		=	62500		; set CompareA to 8 sec. delay
ATA_Source_DRDY		=	TIMER1_CK_1024	; prescaling source for DRDY

ATA_Time_ClearBuffer	=	62500		; set CompareA to 8 sec. delay
ATA_Source_ClearBuffer	=	TIMER1_CK_1024	; prescaling source for ClearBuffer
		
;***********************************************************************
;* Time Definitions
;***********************************************************************
;  allowed values for ATA_StandyTime
; 0		= Timeout disabled
; 1-240		= (value*5)sec
; 241-251	= ((value-240)*30)min
; 252		= 21 min
; 253		= Period between 8 and 12 hrs
; 254		= Reserved
; 255		= 21min 15sec
ATA_StandyTime	= 18	; 90 sec
			
		
;***********************************************************************
;* Debug Level  Definitions
;***********************************************************************
;*
;* ata_debug_high	-	all debug code is active
;* ata_debug_medium	-	short debug code is active	
;* ata_debug_low	-	only important debug code is active	
;*
;  ifdef	ata_debug_high
;ata_debug_medium	
;ata_debug_low		
;  endif
;  ifdef	ata_debug_medium
;   ifndef ata_debug_high
;ata_debug_low		
;   endif
;  endif


;***********************************************************************
;* Debug Level Includes Definitions
;***********************************************************************
;  ifdef	ata_debug_low
;	ifndef include_convert
;include_convert		
;	endif	
;	ifndef	include_uartext
;include_uartext		
;	endif
;  endif
;***********************************************************************

;  ifdef	include_convert
;	SEGMENT DATA
;	ifndef DATA1
;; Data1 & Data2 are needed for convertion functions
;DATA1:	res	32
;DATA2:	res	32
;	endif
;  endif     	


;**********************************************************************************
		restore
	endif

;*
;* $Log: atadef.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
