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
;* $Id: atapidef.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : ATAPI Definitions
;*  Version         : 1.5
;*
;*  Filename        : atapidef.inc
;*  Created         : 22.04.2000 by Jens Funke
;*  Last changed    : 15.08.2000 by Jens Funke
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : ATAPI Memory-Address
;*
;*  Homepage    : www.MP3ve.de
;*		  www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************

   	ifndef  atapidefinc       ; prevent multiple binding
atapidefinc       equ     1

		save
                ;listing off

                if       mompass=1
                 message "ATAPI Definitions V1.5 (C) 2000 by Jens Funke"
                endif

;***********************************************************************
;* ATAPI-Interface Adresses
;***********************************************************************
; /CS1Fx aktiv
ATAPI_RWADataLow  	EQU       0b1100000000000000      ;1f0    RW: Data Low ATAPI
ATAPI_WAFeature   	EQU       0b1100000000000001      ;1f1    W: Feature Reg.
ATAPI_RAError     	EQU       0b1100000000000001      ;1f1    R: Error Reg.
ATAPI_RWAInterrupt 	EQU       0b1100000000000010      ;1f2:   RW: Interrupt Reason/SectorCount Reg.
ATAPI_RWASecNo    	EQU       0b1100000000000011      ;1f3:   RW: Sectornumber Reg.
ATAPI_RWACylLow   	EQU       0b1100000000000100      ;1f4:   RW: Cylinder-low Reg.
ATAPI_RWACylHigh  	EQU       0b1100000000000101      ;1f5:   RW: Cylinder-high Reg.
ATAPI_RWADrvSel   	EQU       0b1100000000000110      ;1f6:   RW: Drive Select Reg.
ATAPI_WACommand   	EQU       0b1100000000000111      ;1f7:   W: Command
ATAPI_RAStatus    	EQU       0b1100000000000111      ;1f7:   R: Status

; /CS3Fx aktiv
ATAPI_RWADataHigh 	EQU       0b1100000000001000      ;3f0    RW: Data HIGH Latch
;ATAPI_RAFloppyA   	EQU       0b1100000000001000      ;3f0    R: Floppy A Status
ATAPI_RAFloppyB   	EQU       0b1100000000001001      ;3f1    R: Floppy B Status
ATAPI_RAFloppyDOut   	EQU       0b1100000000001010      ;3f2 	  W: Floppy Digital Output
ATAPI_RAFloppyID  	EQU       0b1100000000001011      ;3f3    R: Floppy ID / Tape Control
ATAPI_RAFloppyStat    	EQU       0b1100000000001100      ;3f4    R: Floppy Controller Stauts
ATAPI_RWAFloppyData   	EQU       0b1100000000001101      ;3f5    RW: Floppy Data Register
ATAPI_RAAltStatus 	EQU       0b1100000000001110      ;3f6:   R: Alternate Status
ATAPI_WADevCtr    	EQU       0b1100000000001110      ;3f6:   W: Device Control
ATAPI_RAAddress   	EQU       0b1100000000001111      ;3f7:   R: Change / Dirve Address


;***********************************************************************
;* ATA Commands
;***********************************************************************
ATAPI_ChkPwrMode	EQU	0xDB		; Check Power Mode
ATAPI_DrvDiags		EQU	0x90		; Execute Drive Diagnostic
ATAPI_IdleImm		EQU	0xE1		; Idle Immediate
ATAPI_Nop		EQU	0x00		; NOP
ATAPI_Packet		EQU	0xA0		; Packet Command
ATAPI_IDDev		EQU	0xA1		; Identify Device
ATAPI_Reset		EQU	0x08		; Soft Reset
ATAPI_Service		EQU	0xA2		; Service
ATAPI_SetFeature	EQU	0xEF		; Set Features
ATAPI_Sleep		EQU	0xE6		; Sleep
ATAPI_StandbyImm	EQU	0xE0		; Standby Immediate

;***********************************************************************
;* Packet Commands
;***********************************************************************
ATAPI_TestUnitReady	EQU	0x00		; Test Unit Ready
ATAPI_ReqSense		EQU	0x03		; Request Sense
ATAPI_Format		EQU	0x04		; Format Unit
ATAPI_Inquiry		EQU	0x12		; Inquiry
ATAPI_StartStop		EQU	0x1B		; Start Stop Unit
ATAPI_Removal		EQU	0x1E		; Prevent Allow Medium Removal
ATAPI_ReadCap		EQU	0x25		; Read CD Record Capacity
ATAPI_Read10		EQU	0x28		; Read
ATAPI_Write10		EQU	0x2A		; Write
ATAPI_Seek		EQU	0x2B		; Seek
ATAPI_SyncCache		EQU	0x35		; Syncronize Cache
ATAPI_ReadSub		EQU	0x42		; Read Subchannel
ATAPI_ReadToc		EQU	0x43		; Read TOC
ATAPI_ReadHeader	EQU	0x44		; Read Header
ATAPI_Play10		EQU	0x45		; Play Audio 10
ATAPI_PlayMSF		EQU	0x47		; Play Audio MSF
ATAPI_Pause_Resume	EQU	0x4B		; Pause / Resume
ATAPI_Stop		EQU	0x4E		; Stop Play / Scan
ATAPI_ReadDisc		EQU	0x51		; Read Disc Information (CD-R/RW)
ATAPI_ReadTrack		EQU	0x52		; Read Track Information
ATAPI_ReserveTrack	EQU	0x53		; Reserve Track
ATAPI_ModeSel		EQU	0x55		; Mode Select 10
ATAPI_SendOPC		EQU	0x54		; Send OPC Information
ATAPI_RepairTrack	EQU	0x58		; Repair Track
ATAPI_ReadMaster	EQU	0x59		; Read Master CUE
ATAPI_ModeSense		EQU	0x5A		; Mode Sense 10
ATAPI_Close		EQU	0x5B		; Close Track / Session
ATAPI_ReadBuffer	EQU	0x5C		; Read Buffer Capacity
ATAPI_SendCUE		EQU	0x5D		; Send CUE Sheet
ATAPI_Blank		EQU	0xA1		; Blank (erase CD-RW)
ATAPI_Play12		EQU	0xA5		; Play Audio 12
ATAPI_Load		EQU	0xA6		; Load / Unload CD
ATAPI_Read12		EQU	0xA8		; Read 12
ATAPI_Write12		EQU	0xAA		; Write 12
ATAPI_SetSpeed		EQU	0xBB		; Set CD Speed
ATAPI_ReadMSF		EQU	0xB9		; Read CD MSF
ATAPI_Scan		EQU	0xBA		; Scan
ATAPI_PlayCD		EQU 	0xBC		; Play CD
ATAPI_MechStatus	EQU	0xBD		; Mechanism Status
ATAPI_ReadCD		EQU	0xBE		; Read CD

;***********************************************************************
;* Bit Definitions
;***********************************************************************
; Error Register
ATAPI_ILI	EQU	0
ATAPI_EOM	EQU	1
ATAPI_ABRT	EQU	2
ATAPI_MCR	EQU	3
ATAPI_SENSE0	EQU	4
ATAPI_SENSE1	EQU	5
ATAPI_SENSE2	EQU	6
ATAPI_SENSE3	EQU	7

;Drive/Head Register
;ATAPI_LUN0	EQU	0	; obsolete
;ATAPI_LUN1	EQU	1	; obsolete
;ATAPI_LUN2	EQU	2	; obsolete
;ATAPI_LUN3	EQU	3	; obsolete
ATAPI_DRV	EQU	4

;Status Register
ATAPI_CHECK	EQU	0
ATAPI_DRQ	EQU	3
ATAPI_SERV	EQU	4
ATAPI_DMARDY	EQU	5
ATAPI_DRDY	EQU	6
ATAPI_BSY	EQU	7

;Device Control Register
ATAPI_nIEN	EQU	1
ATAPI_SRST	EQU	2

; Interrupt Reason Register
ATAPI_CoD	EQU	0
ATAPI_IO	EQU	1
ATAPI_REL 	EQU	2

; Feature Register
ATAPI_DMA	EQU	0
ATAPI_OVERLAP 	EQU	1

;***********************************************************************
;* Global ATAPI-Variables
;***********************************************************************
	SEGMENT DATA 
; Error Register
ATAPI_Error		res	1 ; contains the value of the Error Register 
				  ; from the Device	 

; Sense Key data 
ATAPI_SenseKey		res	18

;***********************************************************************
;* Ticks & Source for Timeout Definitions
;***********************************************************************
; To calculate XXX_Time_XXX use:	
; XXX_Time_XXX = Time(sec) * Clk(Hz) / prescaling source
; use TIMER1_CK,TIMER1_CK_8, TIMER1_CK_64, TIMER1_CK_256 or TIMER1_CK_1024
; for prescaling source	  
;***********************************************************************
ATAPI_Time_NBSY		=	62500		; set CompareA to 8 sec. delay
ATAPI_Source_NBSY	=	TIMER1_CK_1024	; prescaling source for NBSY
ATAPI_Time_INTRQ	=	62500		; set CompareA to 8 sec. delay
ATAPI_Source_INTRQ	=	TIMER1_CK_1024	; prescaling source for INTRQ
	
ATAPI_Time_Init_Media	=	62500		; set CompareA to 8 sec. delay
ATAPI_Source_Init_Media =	TIMER1_CK_1024	; prescaling source for Init_Media 

  ifdef	atapi_slow_device
ATAPI_Time_Load_Disc	=	62500		; set CompareA to 8 sec. delay
ATAPI_Source_Load_Disc	=	TIMER1_CK_1024	; prescaling source for Load_Disc
  endif	
	
ATAPI_Time_ClearBuffer  =	62500		; set CompareA to 8 sec. delay
ATAPI_Source_ClearBuffer =	TIMER1_CK_1024	; prescaling source for ClearBuffer
		
;***********************************************************************
;* Command options
;***********************************************************************
ATAPI_Stop_Disc		equ	0x00
ATAPI_Start_Disc	equ	0x01
ATAPI_Eject_Disc	equ	0x02
ATAPI_Load_Disc		equ	0x03

ATAPI_Scan_LBA		equ	0b00000000
ATAPI_Scan_AbsTime	equ	0b01000000
ATAPI_Scan_TrackNr	equ	0b10000000
ATAPI_Scan_Forward	equ	0b00010000
ATAPI_Scan_Reverse	equ	0b00000000

ATAPI_Resume		equ	0x01
ATAPI_Pause		equ	0x00

ATAPI_Read_TOC_MSF	equ	0b00000010
ATAPI_Read_TOC_LBA	equ	0b00000000
ATAPI_Read_TOC_FirstTrack	equ	0b00000000
ATAPI_Read_TOC_MultiSession	equ	0b01000000
ATAPI_Read_TOC_SubChannel	equ	0b10000000


;***********************************************************************
;* Additional Sense Code Definitions, see ATAPI-Specification
;***********************************************************************
ATAPI_ASC_Medium_May_Have_Changed	equ	0x28
ATAPI_ASC_PowerOn_Reset			equ	0x29
ATAPI_ASC_Medium_Not_Present		equ	0x3A

;***********************************************************************
;* Debug Level  Definitions
;***********************************************************************
;*
;* atapi_debug_high	-	all debug code is active
;* atapi_debug_medium	-	short debug code is active	
;* atapi_debug_low	-	only important debug code is active	
;*	
;  ifdef	atapi_debug_high
;atapi_debug_medium	
;atapi_debug_low		
;  endif
;  ifdef	atapi_debug_medium
;   ifndef		atapi_debug_high
;atapi_debug_low		
;   endif
;  endif
	
;***********************************************************************
;* Debug Level Includes Definitions
;***********************************************************************
;  ifdef	atapi_debug_low
;	ifndef	include_convert
;include_convert		
;	endif
;	ifndef	include_uartext
;include_uartext		
;	endif
;  endif
	
;***********************************************************************
;*
;* ifdef all_code
;* ifdef atapi_audio
;*
;*
;*
;*



	
;***********************************************************************
;* Debug Level  Definitions
;***********************************************************************
  ifdef	include_convert
	SEGMENT DATA
	ifndef DATA1
; Data1 & Data2 are needed for convertion functions
DATA1:	res	32
DATA2:	res	32
	endif
  endif     	


	
;**********************************************************************************
		restore
	endif
;*
;* $Log: atapidef.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
