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
;* $Id: devdef.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Device definitions
;*  Version     : 1.5
;*				
;*  Filename    : devdef.inc
;*  Created     : 28/11/2000 by Jens Funke
;*  Last changed: 26/03/2001 by Jens Funke
;*	
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : -
;*
;*  Homepage    : www.MP3ve.de
;*                www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : -
;*
;***********************************************************************
   	ifndef  devdefinc       ; prevent multiple binding
devdefinc       equ     1

		save
                listing on

                if       mompass=1
                 message "Device Definitions V1.5 (C) 2000 by Jens Funke"
                endif

;***********************************************************************
;* Global ATAPI-Variables
;***********************************************************************
	SEGMENT DATA

DEV_MasterErrorFlags	res	1 
; Bits of DEV_MasterErrorFlags
DEV_SpezialError	equ	0
DEV_MediumError		equ	1
DEV_HardwareError	equ	2
DEV_TimeoutError	equ	3
DEV_IlleagalRequest	equ	4
DEV_RecoveredError	equ	5 ; successfully but recovery action performed
DEV_Retry		equ	6
DEV_FatalError		equ	7

DEV_SlaveErrorFlags	res	1 
; Bits of DEV_SlaveErrorFlags
;DEV_SpezialError	equ	0
;DEV_MediumError	equ	1
;DEV_HardwareError	equ	2
;DEV_TimeoutError	equ	3
;DEV_IlleagalRequest	equ	4
;DEV_RecoveredError	equ	5 ; successfully but recovery action performed
;DEV_Retry		equ	6
;DEV_FatalError		equ	7

	
; Status Flags
DEV_MasterDrvStatusFlags	res	1	
; Bits of DEV_MasterDrvStatusFlags
DEV_DisabledInt		equ	1	; enabled = 0, disabled = 1
DEV_Disc_Loaded		equ	2	; loaded = 1, empty = 0
DEV_Medium_Changed	equ	4	; changed = 1, unchanged = 0	
DEV_Tray_Open		equ	5	; tray is open = 1, tray is closed = 0
DEV_LBA_Mode		equ	6	; LBA = 1, CHS = 0 (only for disc drives)
DEV_Audio_Medium	equ	7	; Audio = 1, Data = 0

DEV_SlaveDrvStatusFlags	res	1	
; Bits of DEV_MasterDrvStatusFlags
;DEV_DisabledInt	equ	1	; enabled = 0, disabled = 1
;DEV_Disc_Loaded	equ	2	; loaded = 1, empty = 0
;DEV_Medium_Changed	equ	4	; changed = 1, unchanged = 0 
;DEV_Tray_Open		equ	5	; tray is open = 1, tray is closed = 0
;DEV_Audio_Medium	equ	7	; Audio = 1, Data = 0

	
DEV_StatusFlags		res	1
; Bits of DEV_StatusFlags
DEV_MasterAvailable	equ	2	; available = 1
DEV_SlaveAvailable	equ	3	; available = 1
DEV_DrvSelected		equ	4	; Master = 0, Slave = 1 
DEV_SlaveTyp		equ	6	; ATAPI = 0, ATA = 1
DEV_MasterTyp		equ	7	; ATAPI = 0, ATA = 1

; currend speed
DEV_MasterSpeed_LSB		res	1	; value in word means KByte/sec 
DEV_MasterSpeed_MSB		res	1
DEV_SlaveSpeed_LSB		res	1	; value in word means KByte/sec 
DEV_SlaveSpeed_MSB		res	1

; Command data
DEV_TempSector			res	2048		; temp sector
	
;only for temp use	
DEV_Identify_Data		=	DEV_TempSector	; Identify data of the selected device (512 byte)

DEV_Inquiry_Data		=	DEV_TempSector	; Inquiry data of the selected device (36 byte)

DEV_MechStatus_Data		=	DEV_TempSector	; Mechanism Status command data (12 byte)

DEV_ReadCap_Data		=	DEV_TempSector	; Read CD Capacitiy command data (8 byte)

DEV_Null_Sector			=	DEV_TempSector	; nirvana, all data will be lost

; data structure for copy
DEV_Source_Device		res	1 	; for definition see mp3.inc "MP3_Source"
DEV_Dest_Device 		res	1 	; for definition see mp3.inc "MP3_Source"

DEV_Source_StartSector3		res	1	; (MSB)
DEV_Source_StartSector2		res	1
DEV_Source_StartSector1		res	1
DEV_Source_StartSector0		res	1	; (LSB)

DEV_Source_Len3			res	1	; (MSB)
DEV_Source_Len2			res	1
DEV_Source_Len1			res	1
DEV_Source_Len0			res	1	; (LSB)
	
DEV_Dest_StartSector3		res	1	; (MSB)
DEV_Dest_StartSector2		res	1
DEV_Dest_StartSector1		res	1
DEV_Dest_StartSector0		res	1	; (LSB)

DEV_CopyBuffer		=	DEV_TempSector		

DEV_Master4Copy		=	0b00000000
DEV_Slave4Copy		=	0b00010000
		
;***********************************************************************
;* Device-Interface Adresses
;***********************************************************************
DEV_RWADataLow		=	ATAPI_RWADataLow	;=	ATA_RWADataLow
DEV_WAFeature		=	ATAPI_WAFeature		;=	ATA_WAFeature
DEV_RAError		=	ATAPI_RAError		;=	ATA_RAError
DEV_RWASecNo		=	ATAPI_RWASecNo		;=	ATA_RWASecNo
DEV_RWACylLow		=	ATAPI_RWACylLow		;=	ATA_RWACylLow
DEV_RWACylHigh		=	ATAPI_RWACylHigh	;=	ATA_RWACylHigh
DEV_RWADrvSel		=	ATAPI_RWADrvSel		;=	ATA_RWADevHead
DEV_WACommand		=	ATAPI_WACommand		;=	ATA_WACommand
DEV_RAStatus		=	ATAPI_RAStatus		;=	ATA_RAStatus
	
DEV_RWADataHigh		=	ATAPI_RWADataHigh	;=	ATA_RWADataHigh
DEV_RAAltStatus		=	ATAPI_RAAltStatus	;=	ATA_RAAltStatus
DEV_WADevCtr		=	ATAPI_WADevCtr		;=	ATA_WAContol
	
DEV_HWResetPort		= 	PORTB
DEV_HWResetPin		= 	PB4

;***********************************************************************
;* Device Bit Definitions
;***********************************************************************
; Error Register
DEV_ABRT	EQU	2	;EQU	ATA_ABRT	EQU	ATAPI_ABRT
	
;Device Control Register
DEV_nIEN	EQU	1	;EQU	ATA_nIEN	EQU	ATAPI_nIEN
DEV_SRST	EQU	2	;EQU	ATA_SRST	EQU	ATAPI_SRST
	
;Status Register
DEV_ERR		EQU	0	;EQU	ATA_ERR		EQU	ATAPI_CHECK
DEV_DRQ		EQU	3	;EQU	ATA_DRQ		EQU	ATAPI_DRQ
DEV_DRDY	EQU	6	;EQU	ATA_DRDY	EQU	ATAPI_DRDY
DEV_BSY 	EQU	7	;EQU	ATA_BSY 	EQU	ATAPI_BSY
	
;***********************************************************************
;* Ticks & Source for Time Definitions
;***********************************************************************
; To calculate XXX_Time_XXX use:	
; XXX_Time_XXX = Time(sec) * Clk(Hz) / prescaling source
; use TIMER1_CK,TIMER1_CK_8, TIMER1_CK_64, TIMER1_CK_256 or TIMER1_CK_1024
; for prescaling source	  
;***********************************************************************
DEV_TIME_HWReset	=	240		; set CompareA to 30 usec. delay	
DEV_Source_HWReset	=	TIMER1_CK	; prescaling source for HWReset

DEV_TIME_SWReset	=	3000		; set CompareA to 3 msec. delay
DEV_Source_SWReset	=	TIMER1_CK_8	; prescaling source for SWReset
				
;***********************************************************************
;* Debug Level  Definitions
;***********************************************************************
;*
;* dev_debug_high	-	all debug code is active
;* dev_debug_medium	-	short debug code is active	
;* dev_debug_low	-	only important debug code is active	
;*
;  ifdef	dev_debug_high
;dev_debug_medium	
;dev_debug_low		
;  endif
;  ifdef	dev_debug_medium
;   ifndef dev_debug_high
;dev_debug_low		
;   endif
;  endif


;***********************************************************************
;* Debug Level Includes Definitions
;***********************************************************************
;  ifdef	dev_debug_low
;	ifndef	include_convert
;include_convert	
;      	INCLUDE "convert.inc"
;	endif
;  endif
;***********************************************************************
;  ifdef	include_convert
;	SEGMENT DATA
;	ifndef DATA1
; Data1 & Data2 are needed for convertion functions
;DATA1:	res	32
;DATA2:	res	32
;	endif
;  endif     	

;***********************************************************************
;* DEV_Command Offset
;***********************************************************************
  ifdef	dev_command_handler_enabled

DEV_ClearBuffer_Offset		=	0
DEV_Init_Media_CMD_Offset	=	4
	
;***********************************************************************
;* DEV_Command JumpTable
;***********************************************************************
	SEGMENT CODE

DEV_Commands:
	data	ATA_ClearBuffer
	data	ATAPI_ClearBuffer

	data	ATA_Init_Media_CMD
	data	ATAPI_Init_Media_CMD
  endif
	
;**********************************************************************************
		restore
	endif

;*
;* $Log: devdef.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
