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
;* $Id: atapi.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: ATAPI Commands
;*  Version     : 1.5
;*
;*  Filename    : atapi.inc
;*  Created     : 04.05.2000 by Jens Funke
;*  Last changed: 07.04.2001 by Marc Dressler
;*
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : ATAPI Commands
;*
;*  Homepage    : www.MP3ve.de
;*		  www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : atapi.mac
;*		  atapiser.inc
;*		  atapidef.inc
;*		  dev.inc
;*		  error.inc
;*		  timer.inc
;*
;***********************************************************************
     	ifndef  atapiinc       ; prevent multiple binding
atapiinc       =     	1
	
		save
;		listing off

                if      MOMPASS=1
                 message "ATAPI Commands V1.5 (C) 2000 by Jens Funke"
		endif

;***********************************************************************
;* Debug IDs
;***********************************************************************
;*
;* To enable the output of a debug message define the message ID below.
;* Comment out message IDs to disable a debug message.
;*
;***********************************************************************
;	
;ATAPI_Init_ID			= 0x0400
;ATAPI_ReqSense_ID		= 0x0401
;ATAPI_StartStop_ID		= 0x0402
;ATAPI_Load_Disc_ID		= 0x0403		
;ATAPI_Eject_Disc_ID		= 0x0404
;ATAPI_Stop_Disc_ID		= 0x0405
;ATAPI_Start_Disc_ID		= 0x0406
;ATAPI_Read10_ID		= 0x0407
;ATAPI_Test_Unit_Ready_ID	= 0x0408
;ATAPI_Inquiry_ID		= 0x0409
;ATAPI_Set_CD_Speed_ID		= 0x0410
;ATAPI_Play_Audio_ID		= 0x0411
;ATAPI_Scan_ID			= 0x0412
;ATAPI_Stop_Play_Scan_ID	= 0x0413			
;ATAPI_Pause_Resume_ID		= 0x0414						
;ATAPI_Read_Toc_ID		= 0x0415
;ATAPI_MechStatus_ID		= 0x0416
;ATAPI_ReadCap_ID		= 0x0417
;	
;ATAPI_CheckCondition_ID	= 0x0430
;ATAPI_Get_MechStatus_ID	= 0x0431
;ATAPI_Get_Capacity_ID		= 0x0432
;ATAPI_Test_Media_ID		= 0x0433
;ATAPI_Init_Media_ID		= 0x0434					
;_ID		= 0x0435
;_ID		= 0x0436
;ATAPI_Set_Speed_ID		= 0x0437
;ATAPI_ClearBuffer_ID		= 0x0438				
;ATAPI_ForceIdleBus_ID		= 0x0439
	
		
;***********************************************************************
;* INCLUDE FILES
;***********************************************************************
	INCLUDE "atapi.mac"
        INCLUDE "atapiser.inc"
        INCLUDE "atapidef.inc"
	INCLUDE "error.inc"
	INCLUDE "timer.inc"
	
	SEGMENT CODE

;***********************************************************************
;* DEFINITIONS
;***********************************************************************

  ifdef atapi_minspeed
ATAPI_Speed_Master	= 300
ATAPI_Speed_Slave	= 300
  endif
	
  ifndef atapi_minspeed
ATAPI_Speed_Master	= 0xffff
ATAPI_Speed_Slave	= 0xffff
  endif
	
;***********************************************************************
;* FUNCTION	: ATAPI_Init
;*
;* CREATED	: 16.07.2000 by Marc Dressler
;*
;* LAST CHANGED : 07.04.2001 by Marc Dressler
;*
;* DESCRIPTION  : Basic ATAPI initialization
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_Init:
	push	param1
	
  ifdef	ATAPI_Init_ID
	DEBUG_MSGID ATAPI_Init_ID
  endif
	
	rcall	ATAPI_Set_Speed

	rcall	DEV_GetDrvStatusFlags_CMD
	cbr	param1,(1<<DEV_Tray_Open)
	cbr	param1,(1<<DEV_Disc_Loaded)
	sbr	param1,(1<<DEV_Medium_Changed)
	rcall	DEV_SetDrvStatusFlags_CMD
	
	pop	param1
	ret

;***********************************************************************
;* Packet Command
;***********************************************************************

;***********************************************************************
;* FUNCTION	: ATAPI_ReqSense_CMD
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 11.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Execute ReqSense
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
ATAPI_ReqSense_CMD:
	push	temp1
	push	temp2
	push	param1
	
  ifdef	ATAPI_ReqSense_ID
	DEBUG_MSGID ATAPI_ReqSense_ID
  endif
	
;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
	ldi     temp1,ATAPI_Packet			; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ldi	param1,18
	ATAPI_SEND_REQSENSE param1, temp1, temp2
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	pop	param1
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_StartStop_CMD
;*
;* CREATED	: 13.05.2000 by Jens Funke
;*
;* LAST CHANGED : 13.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the operation coded in param1 if possible
;*
;* INPUT        : param1:  ATAPI_Stop_Disc	= Stop;
;*			   ATAPI_Start_Disc	= Start the Disc and read the TOC
;*			   ATAPI_Eject_Disc	= Eject the Disc if possible
;*			   ATAPI_Load_Disc	= Load the Disc (Close Tray)
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_StartStop_CMD:
	push	temp1
	push	temp2
        
  ifdef	ATAPI_StartStop_ID
	DEBUG_MSGID ATAPI_StartStop_ID
  endif
;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_SEND_STARTSTOP param1, temp1, temp2
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Load_Disc_CMD
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 12.08.2000 by Jens Funke
;*
;* DESCRIPTION  : Load the Disc if possible
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*	
;***********************************************************************
ATAPI_Load_Disc_CMD:
	push	param1
	
  ifdef	ATAPI_Load_Disc_ID
	DEBUG_MSGID ATAPI_Load_Disc_ID
  endif

	ldi	param1,ATAPI_Load_Disc
	rcall	ATAPI_StartStop_Cmd
	
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1,0
	brne	ATAPI_Load_Disc_CMD_End		; if command was succsessful 
	rcall	DEV_GetDrvStatusFlags_CMD	; set DEV_Medium_Changed
	sbr	param1,(1<<DEV_Medium_Changed); and clear DEV_Tray_Open
	cbr	param1,(1<<DEV_Tray_Open)
	rcall	DEV_SetDrvStatusFlags_CMD
  ifdef	atapi_slow_device
	push	param2
	; init Timer/Counter1
	ldi	param1,hi(ATAPI_Time_Load_Disc)	; set CompareA (see atapidef.inc)
	ldi	param2,lo(ATAPI_Time_Load_Disc)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATAPI_Source_Load_Disc	; start Timer/Counter1 (see atapidef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand
ATAPI_Load_Disc_CMD_Loop:
	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    ATAPI_Load_Disc_CMD_Loop
	pop	param2
  endif	
ATAPI_Load_Disc_CMD_End:	
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Eject_Disc_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 07.04.2001 by Marc Dressler
;*
;* DESCRIPTION  : Eject the Disc if possible
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Eject_Disc_CMD:
	push	param1
	
  ifdef	ATAPI_Eject_Disc_ID
	DEBUG_MSGID ATAPI_Eject_Disc_ID
  endif

	ldi	param1, ATAPI_Eject_Disc
	rcall	ATAPI_StartStop_Cmd
	
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1, 0
	brne	ATAPI_Eject_Disc_CMD_End	; if command was succsessful
	
	rcall	DEV_GetDrvStatusFlags_CMD
				; set DEV_Medium_Changed and DEV_Tray_Open
	sbr	param1, (1<<DEV_Medium_Changed)+(1<<DEV_Tray_Open)
	cbr	param1, (1<<DEV_Disc_Loaded)	; clear DEV_Disc_Loaded
	rcall	DEV_SetDrvStatusFlags_CMD
	
ATAPI_Eject_Disc_CMD_End:	
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Stop_Disc_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Stop the Disc if possible
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATAPI_Stop_Disc_CMD:
	push	param1
	
  ifdef	ATAPI_Stop_Disc_ID
	DEBUG_MSGID ATAPI_Stop_Disc_ID
  endif

	ldi	param1,ATAPI_Stop_Disc
	rcall	ATAPI_StartStop_Cmd
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Start_Disc_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Start the Disc if possible
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Start_Disc_CMD:
	push	param1
	
  ifdef	ATAPI_Start_Disc_ID
	DEBUG_MSGID ATAPI_Start_Disc_ID
  endif

	ldi	param1,ATAPI_Start_Disc
	rcall	ATAPI_StartStop_Cmd
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Read10_CMD
;*
;* CREATED	: 13.05.2000 by Jens Funke
;*
;* LAST CHANGED : 11.08.2000 by Jens Funke
;*
;* DESCRIPTION  : Read one Sector from LBA and copy it to Z
;*
;* INPUT        : param1 = 	LBA (MSB)
;*		  param2 =	LBA
;*		  param3 =	LBA
;*		  param4 =	LBA (LSB)
;*                Z	 = 	Points to the Buffer for the Sector Data
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Read10_CMD:
	push	temp1
	push	temp2
	push	temp3
	push	temp4
	
  ifdef	ATAPI_Read10_ID
	DEBUG_MSGID ATAPI_Read10_ID
  endif

;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	clr	temp2
	ldi	temp1,1
	ATAPI_SEND_READ10	param1, param2, param3, param4, temp2, temp1, temp3, temp4
	
ATAPI_Read10_Cmd_Loop:
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	ATAPI_TST_DRQ	ATAPI_Read10_Cmd_End	; if DRQ=1 then read the Buffer once
	rcall	ATAPI_Read_Buffer_Once
	ATAPI_TST_DRQ	ATAPI_Read10_Cmd_End	; if DRQ=1 then read more data from Buffer
	rjmp	ATAPI_Read10_Cmd_Loop

ATAPI_Read10_Cmd_End:
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp4
	pop	temp3
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Test_Unit_Ready_CMD
;*
;* CREATED	: 08/07/2000 by Jens Funke
;*
;* LAST CHANGED : 19.08.2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Test_Unit_Ready command
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Test_Unit_Ready_CMD:
	push	temp1
	
  ifdef	ATAPI_Test_Unit_Ready_ID
	DEBUG_MSGID ATAPI_Test_Unit_Ready_ID
  endif

;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_TEST_UNIT_READY	temp1		; send Test_Unit_Ready command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Inquiry_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Inquiry command
;*
;* INPUT        : param1	= Allocation Lenght
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Inquiry_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_Inquiry_ID
	DEBUG_MSGID ATAPI_Inquiry_ID
  endif

	
;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_INQUIRY param1, temp1, temp2	; send Inquiry command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Set_CD_Speed_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Send Set CD Speed command
;*
;* INPUT        : param1	= Read Drive Speed in Kbyte/Second (MSB)
;*		  param2	= Read Drive Speed in Kbyte/Second (LSB)
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Set_CD_Speed_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_Set_CD_Speed_ID
	DEBUG_MSGID ATAPI_Set_CD_Speed_ID
  endif

;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_SET_CD_SPEED  param1, param2, temp1, temp2	; send Set CD Speed command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

  ifdef	atapi_audio
;***********************************************************************
;* FUNCTION	: ATAPI_Play_Audio_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Play Audio command
;*
;* INPUT        : param1	= Logical Block Address (MSB)
;*		  param2	= Logical Block Address
;*		  param3	= Logical Block Address
;*		  param4	= Logical Block Address (LSB)
;*		  param5	= Transfer Lenght (MSB)
;*		  param6	= Transfer Lenght (LSB)
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Play_Audio_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_Play_Audio_ID
	DEBUG_MSGID ATAPI_Play_Audio_ID
  endif

;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_PLAY_AUDIO param1, param2, param3, param4, param5, param6, temp1, temp2	; send Play Audio command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret
	
;***********************************************************************
;* FUNCTION	: ATAPI_Scan_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Scan command
;*
;* INPUT        : param1	= Scan Starting Address Field (MSB)
;*		  param2	= Scan Starting Address Field
;*		  param3	= Scan Starting Address Field
;*		  param4	= Scan Starting Address Field (LSB)
;*		  param5	= Direction:	ATAPI_Scan_Forward	= Scan fast-forward
;*						ATAPI_Scan_Reverse	= Scan fast-reversed
;*		  param6	= Format of address contained in the Scan Starting Address Field
;*				  Format:	ATAPI_Scan_LBA		= Logical Block Address format (default)
;*						ATAPI_Scan_AbsTime	= AMIN, ASEC and AFRAME format
;*						ATAPI_Scan_TrackNr	= Track Number (TNO) format
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Scan_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_Scan_ID
	DEBUG_MSGID ATAPI_Scan_ID
  endif

;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_SCAN  param1, param2, param3, param4, param5, param6, temp1, temp2; send Scan command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Stop_Play_Scan_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Stop Play/Scan command
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Stop_Play_Scan_CMD:
	push	temp1
	push	temp2
	
;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_STOP_PLAY_SCAN temp1, temp2	; send Stop Play/Scan command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Pause_Resume_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Pause/Resume command
;*
;* INPUT        : param1 = Option: ATAPI_Resume	= begin play at the block
;*						  following the last block played
;*				   ATAPI_Pause	= enter the hold track state
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Pause_Resume_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_Pause_Resume_ID
	DEBUG_MSGID ATAPI_Pause_Resume_ID
  endif

;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_PAUSE_RESUME param1, temp1, temp2	; send Pause/Resume command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Read_Toc_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Read TOC command
;*
;* INPUT        : param1	= MSF or LBA
;*				  Options: ATAPI_Read_TOC_MSF = return address in MSF format
;*					   ATAPI_Read_TOC_LBA = return address in LBA format
;*		  param2	= ?????? set to 0x00
;*		  param3	= Starting Track / Session Number
;*		  param4	= Allocation Lenght (MSB)
;*		  param5	= Allocation Lenght (LSB)
;*		  param6	= Format Field
;*				  Options: ATAPI_Read_TOC_FirstTrack
;*					   ATAPI_Read_TOC_MultiSession
;*					   ATAPI_Read_TOC_SubChannel
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Read_Toc_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_Read_Toc_ID
	DEBUG_MSGID ATAPI_Read_Toc_ID
  endif
	
;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_READ_TOC param1, param2, param3, param4, param5, param6, temp1, temp2
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_MechStatus_CMD
;*
;* CREATED	: 09/08/2000 by Jens Funke
;*
;* LAST CHANGED : 09/08/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Mechanism Status command
;*
;* INPUT        : param1 - Alloc Lenght (MSB)
;*		  param2 - Alloc Lenght (LSB)   
;*			  default length is 12 Byte. Dont use more then 12!
;*	
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_MechStatus_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_MechStatus_ID
	DEBUG_MSGID ATAPI_MechStatus_ID
  endif
	
;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1
	
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_Mech_STATUS param1, param2, temp1, temp2	; send Mechanism Status command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_ReadCap_CMD
;*
;* CREATED	: 09/07/2000 by Jens Funke
;*
;* LAST CHANGED : 09/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Execute the Read CD Capacitiy command
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_ReadCap_CMD:
	push	temp1
	push	temp2
	
  ifdef	ATAPI_ReadCap_ID
	DEBUG_MSGID ATAPI_ReadCap_ID
  endif
	
;        lds	temp1,DEV_StatusFlags		; select drive
;	andi	temp1,(1<<DEV_DrvSelected)
;	sts	ATAPI_RWADrvSel,temp1
	
        ldi     temp1,ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand,temp1

	rcall	ATAPI_Wait_NBSY			; wait for NBSY
	ATAPI_READ_CAP temp1, temp2		; send Read CD Capacitiy command
  ifdef	atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	pop	temp2
	pop	temp1
	ret

  endif

;***********************************************************************
;* FUNCTION	: ATAPI_CheckCondition_CMD
;*
;* CREATED	: 10/07/2000 by Jens Funke
;*
;* LAST CHANGED : 07.04.2001 by Marc Dressler
;*
;* DESCRIPTION  : First checks the Status Register then execute Request Sense
;*		  command and stores error reason if nessessary
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_CheckCondition_CMD:
	push	temp1
	push	temp2
	push	temp3
	push	param1
	
  ifdef	ATAPI_CheckCondition_ID
	DEBUG_MSGID ATAPI_CheckCondition_ID
  endif
	
;  ifdef	ATAPI_CheckCondition_ID
;	rcall	DEV_Read_Taskfile
;	rcall	DEV_Print_Taskfile
;  endif
	rcall	ATAPI_Wait_NBSY			; wait for BSY=0
	rcall	DEV_GetErrorFlags_CMD
	sbrc	param1,DEV_TimeoutError		; we could also have a Timeout	
	rjmp	ATAPI_CheckCondition_CMD_Error
	lds	temp1,ATAPI_RAStatus
	sbrc	temp1,ATAPI_CHECK		; if ATAPI_CHECK=0 && DEV_TimeoutError = 0
	rjmp	ATAPI_CheckCondition_CMD_Error
;	rjmp	ATAPI_CheckCondition_CMD_Clear	

ATAPI_CheckCondition_CMD_Clear:			; then clear ErrorFlags and leave
	clr	temp1		
	mov	param1,temp1
	rcall	DEV_SetErrorFlags_CMD
	rjmp	ATAPI_CheckCondition_CMD_End

ATAPI_CheckCondition_CMD_Error:
	lds	temp1,Error			; set Error_fDEV in Error
	sbr	temp1,(1<<Error_fDEV)
	sts	Error,temp1

	rcall	DEV_GetErrorFlags_CMD	
	mov	temp1,param1
	sbrc	temp1,DEV_TimeoutError	; if Timeout Force Reset
	rjmp	ATAPI_CheckCondition_CMD_TimeoutError

	; find out what's happend
	clr	temp2
	sbrc	temp1,ATAPI_MCR
	ldi	temp2,0b01000010		; set DEV_MediumError & DEV_Retry
	sbrc	temp1,ATAPI_ILI
	ldi	temp2,0b10010000		; set IlleagalRequest & FatalError
	sbrc	temp1,ATAPI_EOM
	ldi	temp2,0b10010000		; set IlleagalRequest & FatalError
	mov	param1,temp2
	rcall	DEV_SetErrorFlags_CMD		; store error flags in ErrorFlags
	
	andi	temp1,0b11110000		; we just need the SenseKey
	swap	temp1
	cpi	temp1,0x0
	breq	ATAPI_CheckCondition_CMD_SK0	; if SenseKey = 0
	cpi	temp1,0x1
	breq	ATAPI_CheckCondition_CMD_SK1	; if SenseKey = 1
	cpi	temp1,0x2
	breq	ATAPI_CheckCondition_CMD_SK2	; if SenseKey = 2
	cpi	temp1,0x3
	breq	ATAPI_CheckCondition_CMD_SK3	; if SenseKey = 3
	cpi	temp1,0x4
	breq	ATAPI_CheckCondition_CMD_SK4	; if SenseKey = 4
	cpi	temp1,0x5
	breq	ATAPI_CheckCondition_CMD_SK5	; if SenseKey = 5
	cpi	temp1,0x6
	breq	ATAPI_CheckCondition_CMD_SK6	; if SenseKey = 6
	cpi	temp1,0x7
	breq	ATAPI_CheckCondition_CMD_SK7	; if SenseKey = 7
	cpi	temp1,0xb
	breq	ATAPI_CheckCondition_CMD_SKb	; if SenseKey = b
	cpi	temp1,0xe
	breq	ATAPI_CheckCondition_CMD_SKe	; if SenseKey = e

ATAPI_CheckCondition_CMD_SK0:				; set error flags
	sbr	temp2,(1<<DEV_Retry)
;  ifdef	atapi_debug_medium
;	ldi	param1,'0'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SK1:
	sbr	temp2,(1<<DEV_RecoveredError)
;  ifdef	atapi_debug_medium
;	ldi	param1,'1'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SK2:
	sbr	temp2,(1<<DEV_MediumError)+(1<<DEV_FatalError)
;  ifdef	atapi_debug_medium
;	ldi	param1,'2'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SK3:
	sbr	temp2,(1<<DEV_MediumError)+(1<<DEV_Retry)
;  ifdef	atapi_debug_medium
;	ldi	param1,'3'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SK4:
	sbr	temp2,(1<<DEV_HardwareError)+(1<<DEV_FatalError)
;  ifdef	atapi_debug_medium
;	ldi	param1,'4'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SK5:
	sbr	temp2,(1<<DEV_IlleagalRequest)+(1<<DEV_FatalError)
;  ifdef	atapi_debug_medium
;	ldi	param1,'5'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SK6:
	sbr	temp2,(1<<DEV_Retry)
;  ifdef	atapi_debug_medium
;	ldi	param1,'6'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SK7:
	sbr	temp2,(1<<DEV_SpezialError)+(1<<DEV_FatalError)
;  ifdef	atapi_debug_medium
;	ldi	param1,'7'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SKb:
	sbr	temp2,(1<<DEV_Retry)
;  ifdef	atapi_debug_medium
;	ldi	param1,'b'
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
ATAPI_CheckCondition_CMD_SKe:
	sbr	temp2,(1<<DEV_SpezialError)+(1<<DEV_FatalError)
;  ifdef	atapi_debug_medium
;	ldi	param1,'e'
;  endif
	
;	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense

ATAPI_CheckCondition_CMD_Send_ReqSense:
;  ifdef	atapi_debug_medium
;	UART_PRINT "\n\rSK"
;	rcall	UART_Write
;  endif
	mov	temp3,temp2			; store ErrorFlags in temp3
	ldi	temp2,2				; set loop counter
ATAPI_CheckCondition_CMD_loop:
	rcall	ATAPI_ReqSense_CMD		; send RequestSense
	lds	temp1,ATAPI_RAStatus		; check for errors
	sbrc	temp1,ATAPI_CHECK
	rjmp	ATAPI_CheckCondition_CMD_ForceReset  ; BIG trouble!
	rcall	ATAPI_Read_SenseKey		; store SenseKey
	cbr	temp3,(1<<DEV_TimeoutError)	; clear TimeoutError  
	mov	param1,temp3
	rcall	DEV_SetErrorFlags_CMD		; store DEV_xxxErrorFlags
;  ifdef	atapi_debug_high
;	UART_PRINT "\n\rSense Key: "
;	rcall	ATAPI_Print_SenseKey
;  endif
	; ACHTUNG! It's better to check ATAPI_RAError again!
	rcall	ATAPI_Evaluate_ASC		; set DEV_xxxDrvStatusFlags 
	rcall	ATAPI_Wait_NBSY
	rjmp	ATAPI_CheckCondition_CMD_End    ; leave
	
ATAPI_CheckCondition_CMD_TimeoutError:
	rcall	DEV_SoftwareReset_CMD		; troubleshooting
;  ifdef	atapi_debug_medium
;	ldi	param1,'t'	; timeout error
;  endif
	rjmp	ATAPI_CheckCondition_CMD_Send_ReqSense
	
ATAPI_CheckCondition_CMD_ForceReset:		; try it two times
	cpi	temp2,0
	breq	ATAPI_CheckCondition_CMD_Fatal	; OK, that's it. The device is out of order!
	dec	temp2
	rcall	DEV_SoftwareReset_CMD		; troubleshooting
	rjmp	ATAPI_CheckCondition_CMD_loop

ATAPI_CheckCondition_CMD_Fatal:
	ldi	temp1,0b10000100		; set DEV_HardwareError & DEV_FatalError
	mov	param1,temp1
	rcall	DEV_SetErrorFlags_CMD

ATAPI_CheckCondition_CMD_End:
;   ifdef	atapi_debug_high	
;	ATAPI_PRINT_BYTE_AS_BIN	DEV_MasterErrorFlags
;	ATAPI_PRINT_BYTE_AS_BIN	DEV_SlaveErrorFlags
;   endif
	
	pop	param1
	pop	temp3
	pop	temp2
	pop	temp1
	ret

  ifdef	atapi_audio
;***********************************************************************
;* FUNCTION	: ATAPI_Get_MechStatus_CMD
;*              
;* CREATED	: 10/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 10/08/2000 by Jens Funke
;*              
;* DESCRIPTION  : Execute Mechanism Status command and read ATAPI_MechStatus_Data
;*		  from device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*		  DEV_xxxMechStatus_Data - Mechanism Status command data
;*
;***********************************************************************
ATAPI_Get_MechStatus_CMD:
	push	ZL
	push	ZH
	push	param1
	push	param2
	
  ifdef	ATAPI_Get_MechStatus_ID
	DEBUG_MSGID ATAPI_Get_MechStatus_ID
  endif
	
	ldi	param1,hi(12)
	ldi	param2,lo(12)
	rcall	ATAPI_MechStatus_CMD	; execute Mechanism Status

	rcall	DEV_GetErrorFlags_CMD
	cpi	param1,0
	brne	ATAPI_Get_MechStatus_CMD_End

	SET_ZPOINTER ATAPI_MechStatus_Data	; set pointer 
	rcall	ATAPI_Read_Buffer_Once		; read DEV_xxxMechStatus_Data

ATAPI_Get_MechStatus_CMD_End:	
	pop	param2
	pop	param1
	pop	ZH
	pop	ZL
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Get_Capacity_CMD
;*              
;* CREATED	: 10/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 10/08/2000 by Jens Funke
;*              
;* DESCRIPTION  : Execute Read CD Capacity command and read ATAPI_Read_Cap_Data
;*		  from device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*		  DEV_xxxReadCap_Data   - Read CD Capacity command data
;*
;***********************************************************************
ATAPI_Get_Capacity_CMD:
	push	param1
	push	ZL
	push	ZH
	
  ifdef	ATAPI_Get_Capacity_ID
	DEBUG_MSGID ATAPI_Get_Capacity_ID
  endif
	
	rcall	ATAPI_ReadCap_CMD	; execute Read CD Capacitiy command

	rcall	DEV_GetErrorFlags_CMD
	cpi	param1,0
	brne	ATAPI_Get_Capacity_CMD_End

	SET_ZPOINTER ATAPI_Read_Cap_Data	; set pointer 
	rcall	ATAPI_Read_Buffer_Once		; read ATAPI_MechStatus_Data

ATAPI_Get_Capacity_CMD_End:
	pop	ZH
	pop	ZL
	pop	param1
	ret
  endif
	
;***********************************************************************
;* FUNCTION	: ATAPI_Test_Media_CMD
;*              
;* CREATED	: 10/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 07.04.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Execute Start Disc Command to test the medium
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated ATAPI_Disc_Loaded 
;*
;***********************************************************************
ATAPI_Test_Media_CMD:
	push	param1
	push	temp1
	
  ifdef	ATAPI_Test_Media_ID
	DEBUG_MSGID ATAPI_Test_Media_ID
  endif

	clr	temp1				; reset counter
ATAPI_Test_Media_CMD_Retry:	
	cpi	temp1, 0x03			
	breq	ATAPI_Test_Media_CMD_Start	; after 3 start without setting the speed	
	inc	temp1
	
	rcall	ATAPI_Set_Speed_CMD		; set speed 
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1, 0x00
	brne	ATAPI_Test_Media_CMD_Retry	; try again if we have an error
;	cpi	param1,0
;	brne	ATAPI_Test_Media_CMD_End	; no disc inserted
	
ATAPI_Test_Media_CMD_Start:	
	rcall	ATAPI_Start_Disc_CMD
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1, 0
	breq	ATAPI_Test_Media_CMD_Set
	lds	param1, (ATAPI_SenseKey+12)		; check additional Sense Code
	cpi	param1, ATAPI_ASC_Medium_Not_Present	
	breq	ATAPI_Test_Media_CMD_Clear
	rjmp	ATAPI_Test_Media_CMD_End
		
ATAPI_Test_Media_CMD_Set:	
	rcall	DEV_GetDrvStatusFlags_CMD
	sbr	param1, (1<<DEV_Disc_Loaded)	; set DEV_Disc_Loaded in DEV_xxxDrvStatusFlags
	cbr	param1, (1<<DEV_Medium_Changed)	; clear DEV_Medium_Changed 
	rcall	DEV_SetDrvStatusFlags_CMD	; if Disc is inside
	
	rcall	ATAPI_Determine_MediumType	; determine CD type
	rjmp	ATAPI_Test_Media_CMD_End
	
ATAPI_Test_Media_CMD_Clear:	
	rcall	DEV_GetDrvStatusFlags_CMD
	cbr	param1, (1<<DEV_Disc_Loaded)	; clear DEV_Disc_Loaded in DEV_xxxDrvStatusFlags
	rcall	DEV_SetDrvStatusFlags_CMD	; if Disc isn't inside
	
ATAPI_Test_Media_CMD_End:
;  ifdef	atapi_debug_high
;	ATAPI_PRINT_BYTE_AS_BIN DEV_MasterDrvStatusFlags	
;	ATAPI_PRINT_BYTE_AS_BIN DEV_SlaveDrvStatusFlags	
;  endif
	pop	temp1
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Init_Media_CMD
;*              
;* CREATED	: 11/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 11/08/2000 by Jens Funke
;*              
;* DESCRIPTION  : Test the Unit. If ATAPI_ErrorFlags = 0 we can read from
;*		  the device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated   
;*
;***********************************************************************
ATAPI_Init_Media_CMD:
	push	temp1
	push	param1
	push	param2
	
  ifdef	ATAPI_Init_Media_ID
	DEBUG_MSGID ATAPI_Init_Media_ID
  endif

	rcall	ATAPI_Test_Media_CMD
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1,0
	brne	ATAPI_Init_Media_CMD_End
	
;	ldi	param1,hi(ATAPI_Time_Init_Media) ; set CompareA (see atapidef.inc)
;	ldi	param2,lo(ATAPI_Time_Init_Media)
;	rcall	TIMER1_Write_OCR1A

;        clr	param1			; clear Timer/Counter1
;	clr	param2
;	rcall	TIMER1_Write_TCNT
;	rcall	ATAPI_Source_Init_Media	; start Timer/Counter1 (see atapidef.inc)
;	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand
;ATAPI_Init_Media_CMD_Loop:
;  ifdef	atapi_debug
;	UART_PRINT "*"
;  endif	
;	rcall	ATAPI_Test_Unit_Ready_CMD	; if the unit is ready we can read 
;	rcall	DEV_GetErrorFlags_CMD		; from the device
;	cpi	param1,0
;	breq	ATAPI_Init_Media_CMD_END
		
;	in	temp1,TIFR		; check Timer Overflow of Timer/Counter1
;	sbrc	temp1,OCF1A
;	 rjmp	ATAPI_Init_Media_CMD_End
;	rjmp    ATAPI_Init_Media_CMD_Loop

  ifdef	ATAPI_Init_Media_ID
	push	param4
	rcall	DEV_GetErrorFlags_CMD
	mov	param4,param1
	DEBUG_DATA_MSGID ATAPI_Init_Media_ID
	pop	param4
  endif
			
ATAPI_Init_Media_CMD_End:
	pop	param2
	pop	param1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Set_Speed
;*              
;* CREATED	: 07.04.2001 by Marc Dressler
;*              
;* LAST CHANGED : 07.04.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Set speed = ATAPI_Speed_Master/Slave
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxSpeed_LSB
;*		  DEV_xxxSpeed_MSB
;*
;***********************************************************************
ATAPI_Set_Speed:
	push	temp1
	
  ifdef	ATAPI_Set_Speed_ID
	DEBUG_MSGID ATAPI_Set_Speed_ID
  endif
					; Slave
	ldi	temp1, hi(ATAPI_Speed_Slave)
	sts	DEV_SlaveSpeed_MSB, temp1
	ldi	temp1, lo(ATAPI_Speed_Slave)
	sts	DEV_SlaveSpeed_LSB, temp1
		
					; Master
	ldi	temp1, hi(ATAPI_Speed_Master)
	sts	DEV_MasterSpeed_MSB, temp1
	ldi	temp1, lo(ATAPI_Speed_Master)
	sts	DEV_MasterSpeed_LSB, temp1

	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Set_Speed_CMD
;*              
;* CREATED	: 10/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 10/08/2000 by Jens Funke
;*              
;* DESCRIPTION  : Send DEV_xxxSpeed to the device with value stored in 
;*		  DEV_xxxSpeed_MSB,DEV_xxxSpeed_LSB
;*              
;* INPUT        : DEV_xxxSpeed_MSB
;*		  DEV_xxxSpeed_LSB
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_Set_Speed_CMD:
	push	param1
	push	param2
	
  ifdef	ATAPI_Set_Speed_ID
	DEBUG_MSGID ATAPI_Set_Speed_ID
  endif

	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_DrvSelected		; Master or Slave
	rjmp	ATAPI_Set_Speed_CMD_Master
;	rjmp	ATAPI_Set_Speed_CMD_Slave
	
ATAPI_Set_Speed_CMD_Slave:			; if Slave		
	lds	param1,DEV_SlaveSpeed_MSB
	lds	param2,DEV_SlaveSpeed_LSB
	rjmp	ATAPI_Set_Speed_CMD_Set
		
ATAPI_Set_Speed_CMD_Master:			; if Master	
	lds	param1,DEV_MasterSpeed_MSB
	lds	param2,DEV_MasterSpeed_LSB
;	rjmp	ATAPI_Set_Speed_CMD_Set

ATAPI_Set_Speed_CMD_Set:		
	rcall	ATAPI_Set_CD_Speed_CMD	; send Set CD Speed command
	pop	param2
	pop	param1
	ret
	
;***********************************************************************
;* FUNCTION	: ATAPI_ClearBuffer
;*              
;* CREATED	: 10/08/2000 by Jens Funke
;*              
;* LAST CHANGED : 02/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Abort the active Command and clear all Errors
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATAPI_ClearBuffer:
	push	param1
	push	param2
	
  ifdef	ATAPI_ClearBuffer_ID
	DEBUG_MSGID ATAPI_ClearBuffer_ID
  endif

	; init Timer/Counter1
	ldi	param1,hi(ATAPI_Time_ClearBuffer)	; set CompareA (see atapidef.inc)
	ldi	param2,lo(ATAPI_Time_ClearBuffer)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATAPI_Source_ClearBuffer; start Timer/Counter1 (see atapidef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand
	
ATAPI_ClearBuffer_Loop:	
	lds	param1,ATAPI_RAStatus
	sbrs	param1,ATAPI_DRQ		; leave if DRQ=0
	rjmp	ATAPI_ClearBuffer_End
	
        lds     param1,ATAPI_RWADataLow    	; read ATAPI-Buffer
        lds     param1,ATAPI_RWADataHigh	; read ATAPI-Buffer
	
	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp	ATAPI_ClearBuffer_Loop
	
ATAPI_ClearBuffer_End:	
	
	clr	param1
	rcall	DEV_SetErrorFlags_CMD	; clear all Errors
	
	lds	param1,Error		; clear Error_fDEV in Error
	cbr	param1,(1<<Error_fDEV)
	sts	Error,param1

	pop	param2
	pop	param1
	ret


;***********************************************************************
;* FUNCTION	: ATAPI_ForceIdleBus_CMD
;*              
;* CREATED	: 04/01/2001 by Jens Funke
;*              
;* LAST CHANGED : 04/01/2001 by Jens Funke
;*              
;* DESCRIPTION  : Abort the active Command and force an idle bus
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
ATAPI_ForceIdleBus_CMD:
	
  ifdef	ATAPI_ForceIdleBus_ID
	DEBUG_MSGID ATAPI_ForceIdleBus_ID
  endif

	rcall	ATAPI_ClearBuffer
	rcall	ATAPI_CheckCondition_CMD	; now we need an idle bus
	ret		
	
;***********************************************************************



; ATAPI_Read_SubChannel
;
; 
;
;
;
;

;***********************************************************************
		restore
	endif

;*
;* $Log: atapi.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
