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
;* $Id: ata.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: ATA Commands
;*  Version     : 1.5
;*				
;*  Filename    : ata.inc
;*  Created     : 28/11/2000 by Jens Funke
;*  Last changed: 07.04.2001 by Marc Dressler
;*	
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : -
;*
;*  Homepage    : www.MP3ve.de
;*		  www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : timer.inc
;*		  atadef.inc
;*		  ata.mac
;*		  ataser.inc
;*
;***********************************************************************
     	ifndef  atainc       ; prevent multiple binding
atainc       =     	1
                if      MOMPASS=1
                 message "ATA Commands V1.5 (C) 2000 by Jens Funke"
		endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE	"atadef.inc"
	INCLUDE	"ata.mac"
	INCLUDE "ataser.inc"
	INCLUDE	"timer.inc"

;		ifdef ata_debug_low
;	INCLUDE	"uart.mac"
;		endif

;***********************************************************************
;* Debug IDs
;***********************************************************************
;*
;* To enable the output of a debug message define the message ID below.
;* Comment out message IDs to disable a debug message.
;*
;***********************************************************************
;	
;ATA_Init_ID			= 0x0300
;ATA_Read_ID			= 0x0301 
;ATA_Write_ID			= 0x0302
;ATA_Standby_Immediate_ID	= 0x0303
;ATA_Standby_ID			= 0x0304
;	
;ATA_ErrorFlagsUpdate_ID	= 0x0320
;ATA_ClearBuffer_ID		= 0x0321
;ATA_Dev2Dev_Copy_ID		= 0x0322
;ATA_Init_Media_ID		= 0x0323
;ATA_Identify_Device_ID		= 0x0324
;ATA_Read10_ID			= 0x0325

	
       SEGMENT CODE

;***********************************************************************
;* FUNCTION	: ATA_Init
;*              
;* CREATED	: 30/11/2000 by Jens Funke
;*              
;* LAST CHANGED : 22/03/2001 by Jens Funke
;*              
;* DESCRIPTION  : Initiate ATA devices
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
ATA_Init:
	push	param1
	
  ifdef	ATA_Init_ID
	DEBUG_MSGID ATA_Init_ID
  endif	

ATA_Init_Master:	
	rcall	DEV_Select_Master_CMD
	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_MasterAvailable
	rjmp	ATA_Init_Slave
	
	sbrc	param1,DEV_MasterTyp	
	rcall	ATA_Standby_CMD		; set Standby Counter
	
ATA_Init_Slave:		
	rcall	DEV_Select_Slave_CMD
	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_SlaveAvailable
	rjmp	ATA_Init_End
	
	sbrc	param1,DEV_SlaveTyp	
	rcall	ATA_Standby_CMD		; set Standby Counter
	
ATA_Init_End:	
	pop	param1
	ret
	
;***********************************************************************
;* FUNCTION	: ATA_Read_CMD
;*              
;* CREATED	: 29/11/2000 by Jens Funke
;*              
;* LAST CHANGED : 29/11/2000 by Jens Funke
;*              
;* DESCRIPTION  : Read x sectors from the ATA-Device
;*              
;* INPUT        : param1 = 	LBA (MSB)
;*		  param2 =	LBA
;*		  param3 =	LBA
;*		  param4 =	LBA (LSB)
;*		  param5 =	x (sectorcount)
;*                Z	 = 	Points to the Buffer for the Sector Data
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATA_Read_CMD:
	push	param1
	push	temp1
	
  ifdef	ATA_Read_ID
	DEBUG_MSGID ATA_Read_ID
  endif	

	rcall	DEV_Calulate_DevHead	; now create the Device/Head reg.
	
	rcall	ATA_Wait_DRDY
	
	lds	temp1,ATA_RAStatus	
	sbrc	temp1,ATA_DRQ
	rcall	ATA_ClearBuffer		; buffer must be empty
			
	sts	ATA_RWADevHead,param1		; write Device/Head reg.
	sts	ATA_RWACylHigh,param2		; write Cylinder reg.
	sts	ATA_RWACylLow,param3
	sts	ATA_RWASecNo,param4		; write Sector Number reg
	sts	ATA_RWASecCount,param5		; write Sector Count reg.
	
        ldi     temp1,ATA_ReadSecLoop		
        sts     ATA_WACommand,temp1		; write ReadCommand 	

	rcall	ATA_Read_Buffer_Once
	
ATA_Read_CMD_End:
	rcall	ATA_ErrorFlagsUpdate
	pop	temp1
	pop	param1
	ret
	
	ifdef all_code
;***********************************************************************
;* FUNCTION	: ATA_Write_CMD
;*              
;* CREATED	: 06/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 06/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Write x sectors to the ATA-Device
;*              
;* INPUT        : param1 = 	LBA (MSB)
;*		  param2 =	LBA
;*		  param3 =	LBA
;*		  param4 =	LBA (LSB)
;*		  param5 =	x (sectorcount)
;*                Z	 = 	Points to the Buffer for the Sector Data
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATA_Write_CMD:
	push	param1
	push	temp1

  ifdef	ATA_Write_ID
	DEBUG_MSGID ATA_Write_ID
  endif	

	rcall	DEV_Calulate_DevHead	; now create the Device/Head reg.
	
	rcall	ATA_Wait_DRDY
	rcall   ATA_Wait_NBSY
	
;	lds	temp1,ATA_RAStatus	
;	sbrs	temp1,ATA_DRQ
;	rcall	ATA_ClearBuffer			; buffer must be empty
			
	sts	ATA_RWADevHead,param1		; write Device/Head reg.
	sts	ATA_RWACylHigh,param2		; write Cylinder reg.
	sts	ATA_RWACylLow,param3
	sts	ATA_RWASecNo,param4		; write Sector Number reg
	sts	ATA_RWASecCount,param5		; write Sector Count reg.
	
        ldi     temp1,ATA_WriteSecLoop		
        sts     ATA_WACommand,temp1		; write ReadCommand 	

ATA_Write_CMD_Loop:	
	rcall   ATA_Wait_NBSY
	
	lds	temp1,ATA_RAStatus
	sbrs    temp1,ATA_DRQ			; if DRQ = 0 then leave
	rjmp	ATA_Write_CMD_End

	ld	param1,Z+
	ld	temp1,Z+			; swap DataHigh and DataLow
	sts	ATA_RWADataHigh,temp1		; write DataHigh Register
;		ifdef	ata_debug_high
;	push	param1
;	mov	param1,temp1
;	rcall	UART_WriteHex
;	pop	param1
;		endif

	sts	ATA_RWADataLow,param1		; write DataLow Register
;		ifdef	ata_debug_high
;	rcall	UART_WriteHex
;		endif

	rjmp	ATA_Write_CMD_Loop

ATA_Write_CMD_End:
	rcall	ATA_ErrorFlagsUpdate
	pop	temp1
	pop	param1
	ret
	
	endif
		
;***********************************************************************
;* FUNCTION	: ATA_ErrorFlagsUpdate
;*              
;* CREATED	: 30/11/2000 by Jens Funke
;*              
;* LAST CHANGED : 30/11/2000 by Jens Funke
;*              
;* DESCRIPTION  : Updates the global DEV_xxxErrorFlags
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATA_ErrorFlagsUpdate:
	push	param1
	push	temp2
	
  ifdef	ATA_ErrorFlagsUpdate_ID
	DEBUG_MSGID ATA_ErrorFlagsUpdate_ID
  endif	

	clr	param1
	
	lds	temp2,DEV_RAStatus
	sbrs	temp2,DEV_ERR	; DEV_ERR must be set, else no error occured
	rjmp	ATA_ErrorFlagsUpdate_End
	
	lds     temp2,ATA_RAError		; read device error reg. 
        sbrc    temp2,ATA_ABRT			; check ABRT bit
	sbr	param1,(1<<DEV_FatalError)	; set DEV_FatalError
	
	sbrc	temp2,ATA_IDNF			; check IDNF bit
	sbr	param1,(1<<DEV_FatalError)	; set DEV_FatalError
	
	sbrc	temp2,ATA_MED			; check MED bit
	rjmp	ATA_ErrorFlagsUpdate_MED_ERR	; set 	
	rjmp	ATA_ErrorFlagsUpdate_End
		
ATA_ErrorFlagsUpdate_MED_ERR:	
	sbr	param1,(1<<DEV_FatalError)	; set DEV_FatalError
	sbr	param1,(1<<DEV_MediumError)	; set DEV_MediumError
;	rjmp	ATA_ErrorFlagsUpdate_End
		
ATA_ErrorFlagsUpdate_End:
	cpi	param1,0
	breq	ATA_ErrorFlagsUpdate_SetError
	lds	temp1,Error			; set Error_fDEV in Error
	sbr	temp1,(1<<Error_fDEV)
	sts	Error,temp1

ATA_ErrorFlagsUpdate_SetError:		
	rcall	DEV_SetErrorFlags_CMD
	
;		ifdef	ata_debug_medium
;	push	param1
;	UART_Print "\n\rDEV_ErrorFlags:"
;	rcall	DEV_GetErrorFlags_CMD
;	rcall	UART_WriteHex
;	pop	param1
;		endif
		
ATA_ErrorFlagsUpdate_End1:		
	
	pop	temp2
	pop	param1
	ret


;***********************************************************************
;* FUNCTION	: ATA_ClearBuffer
;*              
;* CREATED	: 03/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 03/12/2000 by Jens Funke
;*              
;* DESCRIPTION  :Read out all data from ATA buffer
;*	ACHTUNG! All data will be lost!	
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
ATA_ClearBuffer:
	push	param1
	push	param2
	
  ifdef	ATA_ClearBuffer_ID
	DEBUG_MSGID ATA_ClearBuffer_ID
  endif	
	
	; init Timer/Counter1
	ldi	param1,hi(ATA_Time_ClearBuffer)	; set CompareA (see atadef.inc)
	ldi	param2,lo(ATA_Time_ClearBuffer)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	ATA_Source_ClearBuffer	; start Timer/Counter1 (see atadef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand
	
	rcall   ATA_Wait_NBSY
ATA_ClearBuffer_Loop:		
	lds	param1,ATA_RAStatus
	sbrs    param1,ATA_DRQ			; if DRQ = 0 then leave
	rjmp	ATA_ClearBuffer_End
	
        lds     param1,ATA_RWADataLow		;Read DataLow Register
        lds     param1,ATA_RWADataHigh		;Read DataHigh Register
	
;		ifdef	ata_debug_high
;	UART_PRINT "*"
;		endif
	
	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp	ATA_ClearBuffer_Loop
	
ATA_ClearBuffer_End:
	clr	param1
	rcall	DEV_SetErrorFlags_CMD	; clear all Errors
	
	lds	param1,Error		; clear Error_fDEV in Error
	cbr	param1,(1<<Error_fDEV)
	sts	Error,param1

	pop	param2
	pop	param1
	ret


	ifdef	all_code
;***********************************************************************
;* FUNCTION	: ATA_Standby_Immediate_CMD
;*              
;* CREATED	: 05/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 05/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Cause the device to enter the Standby mode
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATA_Standby_Immediate_CMD:
	push	param1

  ifdef	ATA_Standby_Immediate_ID
	DEBUG_MSGID ATA_Standby_Immediate_ID
  endif	
	
	rcall	ATA_Wait_DRDY
	
        ldi     param1,ATA_StandbyImm
        sts     ATA_WACommand,param1		; write ReadCommand
	
	rcall	ATA_ErrorFlagsUpdate
	rcall	ATA_Wait_NBSY

	pop	param1
	ret

	endif
;***********************************************************************
;* FUNCTION	: ATA_Standby_CMD
;*              
;* CREATED	: 05/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 05/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Cause the device to enter the Standby mode
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATA_Standby_CMD:
	push	param1
	
  ifdef	ATA_Standby_ID
	DEBUG_MSGID ATA_Standby_ID
  endif	
	
	rcall	ATA_Wait_DRDY
	
	; ACHTUNG! Hard coded time  
	ldi	param1,ATA_StandyTime		; (see atadef.inc) 
	sts	ATA_RWASecCount,param1
	
        ldi     param1,ATA_Standby
        sts     ATA_WACommand,param1		; write ReadCommand
	
	rcall	ATA_ErrorFlagsUpdate
	rcall	ATA_Wait_NBSY

	pop	param1
	ret


  ifdef PleaseUseNewVersion20012001
;***********************************************************************
;* FUNCTION	: ATA_Dev2Dev_Copy_CMD
;*              
;* CREATED	: 06/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 06/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Copy data from Source to Destination
;*              
;* INPUT        : Data structure for copy defined in devdef.inc
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATA_Dev2Dev_Copy_CMD:
	push	param1
	push	param2
	push	param3
	push	param4
	push	param5
	push	param6
	push	param7
	push	param8
	push	temp1
	push	temp2
	push	temp3
	push	temp4
	push	temp5
	push	temp6
	push	temp7
	push	temp8
	push	XL
	push	XH
	push	YL
	push	YH
	push	ZL
	push	ZH
	push	r12
	push	r13
	push	r14
	push	r15
	push	r23
	
;	lds	param1,DEV_StatusFlags	
;	cbr	param1,(1<<DEV_DrvSelected)
	
;	lds	param7,DEV_Source_Device
;	or	param7,param1		; calc. DEV_StatusFlags for the source
	ldi	param1,0x4C	; source = master
	mov	param7,param1
	
;	lds	param8,DEV_Dest_Device
;	or	param8,param1		; calc. DEV_StatusFlags for the destination
	
;	lds	param6,DEV_StatusFlags	; store DEV_StatusFlags
	ldi	param1,0x50	; destination = slave
	mov	param6,param1
	
	lds	temp5,DEV_LBA0_Source_End	; load end address
	lds	temp6,DEV_LBA1_Source_End
	lds	temp7,DEV_LBA2_Source_End
	lds	temp8,DEV_LBA3_Source_End

	lds	XL,DEV_LBA0_Source_Start
	lds	XH,DEV_LBA1_Source_Start
	lds	YL,DEV_LBA2_Source_Start
	lds	YH,DEV_LBA3_Source_Start
		
	lds	r12,DEV_LBA0_Dest_Start
	lds	r13,DEV_LBA1_Dest_Start
	lds	r14,DEV_LBA2_Dest_Start
	lds	r15,DEV_LBA3_Dest_Start

	ldi	temp2,0x01	; 1 to add
	ldi	temp3,0x04	; 4 sectors to write
        ldi     temp4,ATA_WriteSecLoop		
	
ATA_Dev2Dev_Copy_CMD_Loop:	
	mov	param1,XL
	mov	param2,XH
	mov	param3,YL
	mov	param4,YH
			
	SET_ZPOINTER	DEV_CopyBuffer
	sts	DEV_StatusFlags,param7	; select source device
	rcall	DEV_ReadSector_CMD	; read source
	
	lds	temp1,DEV_RAStatus
	
;	push	param1
;	UART_Print "\n\rDEV_RAError:	"
;	mov	param1,temp1
;	rcall	UART_WriteHex
;        rcall   UART_WriteCRLF    
;	pop	param1    
	
	sbrc	temp1,ATAPI_CHECK
	rjmp	ATA_Dev2Dev_Copy_CMD_End
	
	add	YH,temp2
	brcc	ATA_Dev2Dev_Copy_Increas
	add	YL,temp2
	brcc	ATA_Dev2Dev_Copy_Increas
	add	XH,temp2
	brcc	ATA_Dev2Dev_Copy_Increas
	add	XL,temp2
ATA_Dev2Dev_Copy_Increas:

;;		ifdef	ata_debug_medium
;	push	param1
;	UART_Print "\n\rDEV_Source:	"
;        rcall   UART_WriteCRLF
;	mov	param1,XL
;	rcall	UART_WriteHex
;	mov	param1,XH
;	UART_Print "\n\rDEV_LBA1_Source:" 
;	rcall	UART_WriteHex
;	mov	param1,YL
;;	UART_Print "\n\rDEV_LBA2_Source:"
;	rcall	UART_WriteHex
;	mov	param1,YH
;;	UART_Print "\n\rDEV_LBA3_Source:"
;	rcall	UART_WriteHex
;        rcall   UART_WriteCRLF    
;	pop	param1    
;;		endif
	
	SET_ZPOINTER	DEV_CopyBuffer
;	sts	DEV_StatusFlags,param8	; select destination device
	
	or	r12,param6	; now create the Device/Head reg.


;	rcall	ATA_Wait_DRDY
;	rcall   ATA_Wait_NBSY
ATA_Dev2Dev_Copy_Wait	
	lds	temp1,ATA_RAStatus
	sbrc	temp1,ATA_BSY
	rjmp	ATA_Dev2Dev_Copy_Wait
	sbrs	temp1,ATA_DRDY
	rjmp	ATA_Dev2Dev_Copy_Wait
			
;	push	param1
;	UART_Print "\n\rATA_LBA:	"

	sts	ATA_RWADevHead,r12		; write Device/Head reg.
;	mov	param1,r12
;	rcall	UART_WriteHex
	sts	ATA_RWACylHigh,r13		; write Cylinder reg.
;	mov	param1,r13
;	rcall	UART_WriteHex
	sts	ATA_RWACylLow,r14
;	mov	param1,r14
;	rcall	UART_WriteHex
	sts	ATA_RWASecNo,r15		; write Sector Number reg
;	mov	param1,r15
;	rcall	UART_WriteHex
;	pop	param1
	
;	ldi	temp3,0x04	; 4 sectors to write
	sts	ATA_RWASecCount,temp3		; write Sector Count reg.
	
;        ldi     temp4,ATA_WriteSecLoop		
        sts     ATA_WACommand,temp4		; write ReadCommand 	

	
ATA_Dev2Dev_Copy_Write_Loop:
;	rcall   ATA_Wait_NBSY
	lds	temp1,ATA_RAStatus
	sbrc	temp1,ATA_BSY
	rjmp	ATA_Dev2Dev_Copy_Write_Loop

;	lds	temp1,ATA_RAStatus
	sbrs    temp1,ATA_DRQ			; if DRQ = 0 then leave
	rjmp	ATA_Dev2Dev_Copy_Write_End

	ld	param5,Z+
	ld	temp1,Z+		; swap DataHigh and DataLow
	sts	ATA_RWADataHigh,temp1		; write DataHigh Register
;	push	param1
;	mov	param1,temp1
;	rcall	UART_WriteHex
;	pop	param1

	sts	ATA_RWADataLow,param5		; write DataLow Register
;	push	param1
;	mov	param1,param5
;	rcall	UART_WriteHex
;	pop	param1

	rjmp	ATA_Dev2Dev_Copy_Write_Loop
	
ATA_Dev2Dev_Copy_Write_End:	
;	rcall	ATA_ErrorFlagsUpdate
;	lds	temp1,ATA_RAError
	
;	push	param1
;	UART_Print "\n\rATA_RAError:	"
;	mov	param1,temp1
;	rcall	UART_WriteHex
	   
	lds	temp1,ATA_RAStatus
;	UART_Print "\n\rATA_RAStatus:	"
;	mov	param1,temp1
;	rcall	UART_WriteHex
;	pop	param1 
	   
	sbrc	temp1,ATA_ERR
	rjmp	ATA_Dev2Dev_Copy_CMD_End

	add	r15,temp3
	brcc	ATA_Dev2Dev_Copy_Increas_Dest
	add	r14,temp2
	brcc	ATA_Dev2Dev_Copy_Increas_Dest
	add	r13,temp2
	brcc	ATA_Dev2Dev_Copy_Increas_Dest
	add	r12,temp2
ATA_Dev2Dev_Copy_Increas_Dest:

	
	cp	temp5,XL		; compare end & dest. address
	brne	ATA_Dev2Dev_Copy_CMD_Loop_Jump
	cp	temp6,XH
	brne	ATA_Dev2Dev_Copy_CMD_Loop_Jump
	cp	temp7,YL
	brne	ATA_Dev2Dev_Copy_CMD_Loop_Jump
	cp	temp8,YH
	brne	ATA_Dev2Dev_Copy_CMD_Loop_Jump
	rjmp	ATA_Dev2Dev_Copy_CMD_End
	
ATA_Dev2Dev_Copy_CMD_Loop_Jump:
	rjmp	ATA_Dev2Dev_Copy_CMD_Loop
	
ATA_Dev2Dev_Copy_CMD_End:
;	lds	temp1,ATA_RAStatus
;	UART_Print "\n\rATA_RAStatus:	"
;	mov	param1,temp1
;	rcall	UART_WriteHex

	sts	DEV_StatusFlags,param7	; restore DEV_StatusFlags

	pop	r23
	pop	r15
	pop	r14
	pop	r13
	pop	r12
	pop	ZH
	pop	ZL
	pop	YH
	pop	YL
	pop	XH
	pop	XL
	pop	temp8
	pop	temp7
	pop	temp6
	pop	temp5
	pop	temp4
	pop	temp3
	pop	temp2
	pop	temp1
	pop	param8
	pop	param7
	pop	param6
	pop	param5
	pop	param4
	pop	param3
	pop	param2
	pop	param1
	ret
	
  endif
  
;***********************************************************************
;* FUNCTION	: ATA_Init_Media_CMD
;*              
;* CREATED	: 23/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 07.04.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Set DEV_xxxDrvStatus bits. Compareable to the 
;*		  ATAPI_Init_Media_CMD function	 
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxDrvStatusFlags	-	updated
;*		  DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
ATA_Init_Media_CMD:
	push	param1
	push	param2
	push	param3
	push	param4
	push	param5
	
  ifdef	ATA_Init_Media_ID
	DEBUG_MSGID ATA_Init_Media_ID
  endif
	
	ldi	param1, 1
	mov	param5, param1
	clr	param1
	mov	param2, param1
	mov	param3, param1
	mov	param4, param1
	SET_ZPOINTER	DEV_Null_Sector
	rcall	ATA_Read_CMD

	rcall	DEV_GetErrorFlags_CMD
	cpi	param1, 0
	brne	ATA_Init_Media_CMD_Error
	
ATA_Init_Media_CMD_OK:	
	rcall	DEV_GetDrvStatusFlags_CMD
	sbr	param1, (1<<DEV_Disc_Loaded)
	cbr	param1, (1<<DEV_Medium_Changed)+(1<<DEV_Tray_Open)+(1<<DEV_Audio_Medium)
	rcall	DEV_SetDrvStatusFlags_CMD
	rjmp	ATA_Init_Media_CMD_End
		
ATA_Init_Media_CMD_Error:	
	rcall	DEV_GetDrvStatusFlags_CMD

	sbr	param1, (1<<DEV_Medium_Changed)
	cbr	param1, (1<<DEV_Tray_Open)+(1<<DEV_Audio_Medium)+(1<<DEV_Disc_Loaded)
	rcall	DEV_SetDrvStatusFlags_CMD
;	rjmp	ATA_Init_Media_CMD_End
	
ATA_Init_Media_CMD_End:			
	pop	param5
	pop	param4
	pop	param3
	pop	param2
	pop	param1
	ret
	
;***********************************************************************
;* FUNCTION	: ATA_Identify_Device_CMD
;*              
;* CREATED	: 29/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 29/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Enables the host to receive parameter information
;*		  from the device	
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_Identify_Data
;*		  ACHTUNG! DEV_Identify_Data is only temp. stored.
;*			   Use data as fast as possible!
;*
;***********************************************************************
ATA_Identify_Device_CMD:
	push	param1
	
  ifdef	ATA_Identify_Device_ID
	DEBUG_MSGID ATA_Identify_Device_ID
  endif
	
	rcall	ATA_Wait_DRDY
	
        ldi     param1,ATA_IDDev
        sts     ATA_WACommand,param1		; write ATA_IDDev command

	SET_ZPOINTER	DEV_Identify_Data
	rcall	ATA_Read_Buffer_Once	; copy device buffer to SRAM 
		
	rcall	ATA_ErrorFlagsUpdate

	pop	param1
	ret
	

;***********************************************************************
;* FUNCTION	: ATA_Read10_CMD
;*              
;* CREATED	: 14/01/2001 by Jens Funke
;*              
;* LAST CHANGED : 14/01/2001 by Jens Funke
;*              
;* DESCRIPTION  : correspond with ATAPI_Read10_CMD. Read 2048 byte from
;*		  the selected ATA device	
;*              
;* INPUT        : param1 = 	LBA (MSB)
;*		  param2 =	LBA
;*		  param3 =	LBA
;*		  param4 =	LBA (LSB)
;*                Z	 = 	Points to the Buffer for the Sector Data
;*              
;* OUTPUT       : -
;*
;***********************************************************************
ATA_Read10_CMD:
	push	temp1
	
  ifdef	ATA_Read10_ID
	DEBUG_MSGID ATA_Read10_ID
  endif

	rcall	ATA_ATAPISector2ATASector	; calculate sector address
	ldi	temp1,0x04
	mov	param5,temp1			; i need 4 sectors (4 * 512)
	rcall	ATA_Read_CMD			; read from ATA device

	pop	temp1
	ret	
	
; TODO
;			



;***********************************************************************
	endif

;*
;* $Log: ata.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
