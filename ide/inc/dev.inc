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
;* $Id: dev.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Device Functions
;*  Version     : 1.5
;*				
;*  Filename    : dev.inc
;*  Created     : 29/11/2000 by Jens Funke
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
;*  Uses        : devser.inc
;*		  devdef.inc
;*		  ata.inc
;*		  atapi.inc
;*
;***********************************************************************
   	ifndef  devinc       ; prevent multiple binding
devinc       equ     1

		save
                ;listing off

                if       mompass=1
                 message "Device Functions V1.5 (C) 2000 by Jens Funke"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************
	
	INCLUDE	"devdef.inc"
	INCLUDE	"devser.inc"
	
		ifndef	ata_disabled
	INCLUDE "ata.inc"
		endif
		ifndef	atapi_disabled
	INCLUDE	"atapi.inc"
		endif
	
;		ifdef	dev_debug_high
;        INCLUDE "devdebug.inc"
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
;DEV_Init_ID			= 0x0200
;DEV_GetErrorFlags_ID		= 0x0201
;DEV_SetErrorFlags_ID		= 0x0202
;DEV_GetDrvStatusFlags_ID	= 0x0203
;DEV_SetDrvStatusFlags_ID	= 0x0204
;DEV_Enable_Int_ID		= 0x0205
;DEV_Disable_Int_ID		= 0x0206
;DEV_Select_Master_ID		= 0x0207
;DEV_Select_Slave_ID		= 0x0208
;DEV_ReadSector_ID		= 0x0209
;DEV_ClearBuffer_ID		= 0x0210
;DEV_Init_Media_ID		= 0x0211
;DEV_DeviceDetection_ID		= 0x0212	
;DEV_HardwareReset_ID		= 0x0213
;DEV_SoftwareReset_ID		= 0x0214
;DEV_Command_Handler_ID		= 0x0215
;DEV_Copy_ID			= 0x0216
				
												
	
        SEGMENT CODE
;***********************************************************************
;* FUNCTION	: DEV_Init
;*              
;* CREATED	: 01/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Initiate DEV_StatusFlags
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Init:
	
  ifdef	DEV_Init_ID
	DEBUG_MSGID DEV_Init_ID
  endif	
	
	rcall	DEV_DeviceDetection
	
	rcall	ATAPI_Init
	rcall	ATA_Init
	
	ret
	
;***********************************************************************
;* FUNCTION	: DEV_GetErrorFlags_CMD
;*              
;* CREATED	: 01/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Returns the ErrorFlags of the selected device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : param1  -  DEV_xxxErrorFlags
;*
;***********************************************************************
DEV_GetErrorFlags_CMD:
	
  ifdef	DEV_GetErrorFlags_ID
	DEBUG_MSGID DEV_GetErrorFlags_ID
  endif	
	
	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_DrvSelected		; Master or Slave
	rjmp	DEV_GetErrorFlags_CMD_Master
;	rjmp	DEV_GetErrorFlags_CMD_Slave
	
DEV_GetErrorFlags_CMD_Slave:			; if Slave
	lds	param1,DEV_SlaveErrorFlags	; return  SlaveErrorFlags
	rjmp	DEV_GetErrorFlags_CMD_End

DEV_GetErrorFlags_CMD_Master:			; if Master
	lds	param1,DEV_MasterErrorFlags	; return MasterErrorFlags 
;	rjmp	DEV_GetErrorFlags_CMD_End	
		
DEV_GetErrorFlags_CMD_End:
;		ifdef	dev_debug_high
;	rcall	UART_WriteHex
;		endif
	ret

;***********************************************************************
;* FUNCTION	: DEV_SetErrorFlags_CMD
;*              
;* CREATED	: 01/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Set the DEV_xxxErrorFlags of the selected device
;*              
;* INPUT        : param1  -  DEV_xxxErrorFlags
;*              
;* OUTPUT       : -
;*
;***********************************************************************
DEV_SetErrorFlags_CMD:
	push	temp1
	
  ifdef	DEV_SetErrorFlags_ID
	DEBUG_MSGID DEV_SetErrorFlags_ID
  endif	
	
	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected		; Master or Slave
	rjmp	DEV_SetErrorFlags_CMD_Master
;	rjmp	DEV_SetErrorFlags_CMD_Slave
	
DEV_SetErrorFlags_CMD_Slave:			; if Slave
	sts	DEV_SlaveErrorFlags,param1	; store SlaveErrorFlags 
	rjmp	DEV_SetErrorFlags_CMD_End

DEV_SetErrorFlags_CMD_Master:			; if Master
	sts	DEV_MasterErrorFlags,param1	; store MasterErrorFlags
;	rjmp	DEV_SetErrorFlags_CMD_End
		
DEV_SetErrorFlags_CMD_End:
;		ifdef	dev_debug_high
;	rcall	UART_WriteHex
;		endif
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_GetDrvStatusFlags_CMD
;*              
;* CREATED	: 01/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Returns the DEV_xxxDrvStatusFlags of the selected device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : param1  -  DEV_xxxDrvStatusFlags
;*
;***********************************************************************
DEV_GetDrvStatusFlags_CMD:
	
  ifdef	DEV_GetDrvStatusFlags_ID
	DEBUG_MSGID DEV_GetDrvStatusFlags_ID
  endif	

	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_DrvSelected		; Master or Slave
	rjmp	DEV_GetDrvStatusFlags_CMD_Master
;	rjmp	DEV_GetDrvStatusFlags_CMD_Slave
	
DEV_GetDrvStatusFlags_CMD_Slave:		; if Slave
	lds	param1,DEV_SlaveDrvStatusFlags	; return SlaveDrvStatusFlags
	rjmp	DEV_GetDrvStatusFlags_CMD_End

DEV_GetDrvStatusFlags_CMD_Master:		; if Master	
	lds	param1,DEV_MasterDrvStatusFlags ; return MasterDrvStatusFlags
;	rjmp	DEV_GetDrvStatusFlags_CMD_End
		
DEV_GetDrvStatusFlags_CMD_End:
	ret

;***********************************************************************
;* FUNCTION	: DEV_SetDrvStatusFlags_CMD
;*              
;* CREATED	: 01/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Set the DEV_xxxDrvStatusFlags of the selected device
;*              
;* INPUT        : param1  -  DEV_xxxDrvStatusFlags
;*              
;* OUTPUT       : -
;*
;***********************************************************************
DEV_SetDrvStatusFlags_CMD:
	push	temp1
	
  ifdef	DEV_SetDrvStatusFlags_ID
	DEBUG_MSGID DEV_SetDrvStatusFlags_ID
  endif	

	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected		; Master or Slave
	rjmp	DEV_SetDrvStatusFlags_CMD_Master
;	rjmp	DEV_SetDrvStatusFlags_CMD_Slave
	
DEV_SetDrvStatusFlags_CMD_Slave:		; if Slave
	sts	DEV_SlaveDrvStatusFlags,param1	; store SlaveDrvStatusFlags 
	rjmp	DEV_SetDrvStatusFlags_CMD_End

DEV_SetDrvStatusFlags_CMD_Master:		; if Master	
	sts	DEV_MasterDrvStatusFlags,param1	; store MasterDrvStatusFlags	
;	rjmp	DEV_SetDrvStatusFlags_CMD_End
		
DEV_SetDrvStatusFlags_CMD_End:
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Enable_Int_CMD
;*
;* CREATED	: 01/12/2000 by Jens Funke
;*
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*
;* DESCRIPTION  : Enable interrupts of devices
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxDrvStatusFlags  -  clear DEV_DrvStatusFlags
;*
;***********************************************************************
DEV_Enable_Int_CMD:
	push	param1
	
  ifdef	DEV_Enable_Int_ID
	DEBUG_MSGID DEV_Enable_Int_ID
  endif	

	lds	param1,DEV_StatusFlags	
	sbrc	param1,DEV_DrvSelected		; Master or Slave
	rjmp	DEV_Enable_Int_CMD_Slave
	
DEV_Enable_Int_CMD_Master:			; if Master
	sbrs	param1,DEV_MasterAvailable	; check whether Dev is available
	rjmp	DEV_Enable_Int_CMD_NotAvailable	; Dev is not available
	lds	param1,DEV_MasterDrvStatusFlags
	sbr	param1,(1<<DEV_DisabledInt)	; set DEV_DisabledInt
	sts	DEV_MasterDrvStatusFlags,param1
	rjmp	DEV_Enable_Int_CMD_End

	;; Must be tranfered to the device too!!!!!!

DEV_Enable_Int_CMD_Slave:			; if Slave
	sbrs	param1,DEV_SlaveAvailable	; check whether Dev is available
	rjmp	DEV_Enable_Int_CMD_NotAvailable	; Dev is not available
	lds	param1,DEV_SlaveDrvStatusFlags
	sbr	param1,(1<<DEV_DisabledInt)	; set DEV_DisabledInt
	sts	DEV_SlaveDrvStatusFlags,param1
	rjmp	DEV_Enable_Int_CMD_End
	
DEV_Enable_Int_CMD_NotAvailable:		; if Dev is not available
	rcall	DEV_GetErrorFlags_CMD
	sbr	param1,(1<<DEV_FatalError)	; set FatalError
	rcall	DEV_SetErrorFlags_CMD
	
DEV_Enable_Int_CMD_End:		
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Disable_Int_CMD
;*
;* CREATED	: 01/12/2000 by Jens Funke
;*
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*
;* DESCRIPTION  : Disable interrupts of devices
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxDrvStatusFlags  -  set DEV_DrvStatusFlags
;*
;***********************************************************************
DEV_Disable_Int_CMD:
	push	param1
	
  ifdef	DEV_Disable_Int_ID
	DEBUG_MSGID DEV_Disable_Int_ID
  endif	
	
	lds	param1,DEV_StatusFlags	
	sbrc	param1,DEV_DrvSelected		; Master or Slave
	rjmp	DEV_Disable_Int_CMD_Slave
	
DEV_Disable_Int_CMD_Master:			; if Master
	sbrs	param1,DEV_MasterAvailable	; check whether Dev is available
	rjmp	DEV_Enable_Int_CMD_NotAvailable	; Dev is not available
	lds	param1,DEV_MasterDrvStatusFlags
	cbr	param1,(1<<DEV_DisabledInt)	; clear DEV_DisabledInt
	sts	DEV_MasterDrvStatusFlags,param1
	rjmp	DEV_Disable_Int_CMD_End

DEV_Disable_Int_CMD_Slave:			; if Slave
	sbrs	param1,DEV_SlaveAvailable	; check whether Dev is available
	rjmp	DEV_Enable_Int_CMD_NotAvailable	; Dev is not available
	lds	param1,DEV_SlaveDrvStatusFlags
	cbr	param1,(1<<DEV_DisabledInt)	; clear DEV_DisabledInt
	sts	DEV_SlaveDrvStatusFlags,param1
	rjmp	DEV_Disable_Int_CMD_End
	
DEV_Disable_Int_CMD_NotAvailable:		; if Dev is not available
	rcall	DEV_GetErrorFlags_CMD
	sbr	param1,(1<<DEV_FatalError)	; set FatalError
	rcall	DEV_SetErrorFlags_CMD
		
DEV_Disable_Int_CMD_End:		
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Select_Master_CMD
;*
;* CREATED	: 01/12/2000 by Jens Funke
;*
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*
;* DESCRIPTION  : Clear	bit DEV_xxxDrvSelected in DEV_DrvStatusFlags
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxDrvStatusFlags	-	clear DEV_DrvSelected
;*		  DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
DEV_Select_Master_CMD:
	push	param1
	
  ifdef	DEV_Select_Master_ID
	DEBUG_MSGID DEV_Select_Master_ID
  endif	
	
	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_MasterAvailable	; check whether Dev is available	
	rjmp	DEV_Select_Master_CMD_NotAvailable	
	cbr	param1,(1<<DEV_DrvSelected)	; clear DEV_DrvSelected	 
	sts	DEV_StatusFlags,param1

	clr	param1				
	rcall	DEV_Calulate_DevHead
	sts	DEV_RWADrvSel,param1		; select device
	
	rjmp	DEV_Select_Master_CMD_End
	
DEV_Select_Master_CMD_NotAvailable:		; if Dev is not available
	rcall	DEV_GetErrorFlags_CMD
	sbr	param1,(1<<DEV_FatalError)	; set FatalError
	rcall	DEV_SetErrorFlags_CMD
	
DEV_Select_Master_CMD_End:	
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Select_Slave_CMD
;*
;* CREATED	: 01/12/2000 by Jens Funke
;*
;* LAST CHANGED : 01/12/2000 by Jens Funke
;*
;* DESCRIPTION  : Set bit DEV_DrvSelected in DEV_xxxDrvStatusFlags
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxDrvStatusFlags	-	set DEV_DrvSelected
;*		  DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
DEV_Select_Slave_CMD:
	push	param1
	
  ifdef	DEV_Select_Slave_ID
	DEBUG_MSGID DEV_Select_Slave_ID
  endif	
	
	lds	param1,DEV_StatusFlags
	sbrs	param1,DEV_SlaveAvailable	; check whether Dev is available	
	rjmp	DEV_Select_Slave_CMD_NotAvailable	
	sbr	param1,(1<<DEV_DrvSelected)	; clear DEV_DrvSelected	 
	sts	DEV_StatusFlags,param1

	clr	param1				
	rcall	DEV_Calulate_DevHead
	sts	DEV_RWADrvSel,param1		; select device
	
	rjmp	DEV_Select_Slave_CMD_End
	
DEV_Select_Slave_CMD_NotAvailable:		; if Dev is not available
	rcall	DEV_GetErrorFlags_CMD
	sbr	param1,(1<<DEV_FatalError)	; set FatalError
	rcall	DEV_SetErrorFlags_CMD
	
DEV_Select_Slave_CMD_End:	
	pop	param1
	ret


;***********************************************************************
;* FUNCTION	: DEV_ReadSector_CMD
;*              
;* CREATED	: 02/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 02/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Read one Sector (2048Byte) from LBA and copy it to Z
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
DEV_ReadSector_CMD:
  	push	temp1
  	push	param5
	
  ifdef	DEV_ReadSector_ID
	DEBUG_MSGID DEV_ReadSector_ID
  endif	
	
  	lds	temp1,DEV_StatusFlags
  	sbrs	temp1,DEV_DrvSelected		; Master or Slave
  	rjmp	DEV_ReadSector_CMD_Master
  	rjmp	DEV_ReadSector_CMD_Slave

DEV_ReadSector_CMD_Slave:			; ATAPI or ATA
  	sbrs	temp1,DEV_SlaveTyp
  	rjmp	DEV_ReadSector_CMD_ATAPI
  	rjmp	DEV_ReadSector_CMD_ATA
	
DEV_ReadSector_CMD_Master:			; ATAPI or ATA	
  	sbrs	temp1,DEV_MasterTyp
  	rjmp	DEV_ReadSector_CMD_ATAPI
;	rjmp	DEV_ReadSector_CMD_ATA

DEV_ReadSector_CMD_ATA:				; if ATA
 	rcall	ATA_ATAPISector2ATASector	; calculate sector address
  	ldi	temp1,0x04
  	mov	param5,temp1			; i need 4 sectors (4 * 512)
  	rcall	ATA_Read_CMD			; read from ATA device
  	rjmp	DEV_ReadSector_CMD_End			
		
DEV_ReadSector_CMD_ATAPI:			; if ATAPI
 	rcall	ATAPI_Read10_CMD		; read from ATAPI device
;	rjmp	DEV_ReadSector_CMD_End			

DEV_ReadSector_CMD_End:	
  	pop	param5
  	pop	temp1
  	ret

;***********************************************************************
;* FUNCTION	: DEV_ClearBuffer
;*              
;* CREATED	: 03/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 03/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Clear the Buffer of the selected device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
DEV_ClearBuffer:
	
  ifdef	DEV_ClearBuffer_ID
	DEBUG_MSGID DEV_ClearBuffer_ID
  endif
	
  ifdef	dev_command_handler_enabled
	push	temp2

	ldi	temp2,DEV_ClearBuffer_Offset
	rcall	DEV_Command_Handler

	pop	temp2
	ret
  endif
		
  ifndef	dev_command_handler_enabled
  	push	temp1

  	lds	temp1,DEV_StatusFlags
  	sbrs	temp1,DEV_DrvSelected		; Master or Slave
  	rjmp	DEV_ClearBuffer_Master
  	rjmp	DEV_ClearBuffer_Slave
	
DEV_ClearBuffer_Slave:				; ATAPI or ATA
  	sbrs	temp1,DEV_SlaveTyp
  	rjmp	DEV_ClearBuffer_ATAPI
  	rjmp	DEV_ClearBuffer_ATA
	
DEV_ClearBuffer_Master:				; ATAPI or ATA
  	sbrs	temp1,DEV_MasterTyp
  	rjmp	DEV_ClearBuffer_ATAPI
;	rjmp	DEV_ClearBuffer_ATA
	
DEV_ClearBuffer_ATA:				; if ATA
	rcall	ATA_ClearBuffer
  	rjmp	DEV_ClearBuffer_End		
	
DEV_ClearBuffer_ATAPI:				; if ATAPI
  	rcall	ATAPI_ClearBuffer	
;	rjmp	DEV_ClearBuffer_End		
		
DEV_ClearBuffer_End:
  	pop	temp1
  	ret
  endif

;***********************************************************************
;* FUNCTION	: DEV_Init_Media_CMD
;*              
;* CREATED	: 23/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 23/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Check the selected device and initiate 
;*		  DEV_xxxDrvStatusFlags if a medium is loaded  
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags	-	updated
;*
;***********************************************************************
DEV_Init_Media_CMD:
	
  ifdef	DEV_Init_Media_ID
	DEBUG_MSGID DEV_Init_Media_ID
  endif

  ifdef	dev_command_handler_enabled
	push	temp2

	ldi	temp2,DEV_Init_Media_CMD_Offset
	rcall	DEV_Command_Handler

	pop	temp2
	ret
  endif
	
  ifndef	dev_command_handler_enabled
  	push	temp1

  	lds	temp1,DEV_StatusFlags
  	sbrs	temp1,DEV_DrvSelected		; Master or Slave
  	rjmp	DEV_Init_Media_CMD_Master
  	rjmp	DEV_Init_Media_CMD_Slave
	
DEV_Init_Media_CMD_Slave:			; ATAPI or ATA
  	sbrs	temp1,DEV_SlaveTyp
  	rjmp	DEV_Init_Media_CMD_ATAPI
  	rjmp	DEV_Init_Media_CMD_ATA
	
DEV_Init_Media_CMD_Master:			; ATAPI or ATA
  	sbrs	temp1,DEV_MasterTyp
  	rjmp	DEV_Init_Media_CMD_ATAPI
;	rjmp	DEV_Init_Media_CMD_ATA
	
DEV_Init_Media_CMD_ATA:				; if ATA
  	rcall	ATA_Init_Media_CMD
  	rjmp	DEV_Init_Media_CMD_End		
	
DEV_Init_Media_CMD_ATAPI:			; if ATAPI
  	rcall	ATAPI_Init_Media_CMD
;	rjmp	DEV_Init_Media_CMD_End		
		
DEV_Init_Media_CMD_End:
  	pop	temp1
  	ret
  endif		

;***********************************************************************
;* FUNCTION	: DEV_DeviceDetection
;*              
;* CREATED	: 29/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 29/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Try to detect Master and Slave device and set
;*		  DEV_StatusFlags to the correct value 
;*              
;*                Order: Select Master, look for an ATA device,   	
;*			 if error, look for an ATAPI device. Select Slave,
;*			 look for ATA device, if error, look for 
;*			 an ATAPI device.
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_StatusFlags	-	updated
;*		  DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
DEV_DeviceDetection:
	push	param1
	push	temp1
	
  ifdef	DEV_DeviceDetection_ID
	DEBUG_MSGID DEV_DeviceDetection_ID
  endif

	clr	temp1		; clear counter
	rcall	DEV_HardwareReset_CMD	; force a hardware reset to
	rcall	DEV_Wait_5_Seconds	; have a defined state
	
; Check Master 
	lds	temp1,DEV_StatusFlags
	cbr	temp1,(1<<DEV_DrvSelected)	; select Master
	sts	DEV_StatusFlags,temp1
	ldi	temp1,0x00
	sts	DEV_RWADrvSel,temp1	; write Device/Head reg. to select device
					
	rcall	ATA_Wait_NBSY	; I could also call ATAPI_Wait_NBSY, but
				; the ATA_Wait_NBSY-Timeout time is longer (8sec.)
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1,0x00	; check for timeout error
	breq	DEV_DeviceDetection_Seek
	
; Check Slave
	lds	temp1,DEV_StatusFlags
	sbr	temp1,(1<<DEV_DrvSelected)	; select Slave
	sts	DEV_StatusFlags,temp1
	ldi	temp1,0x10
	sts	DEV_RWADrvSel,temp1	; write Device/Head reg. to select device

	rcall	ATA_Wait_NBSY	; I could also call ATAPI_Wait_NBSY, but
				; the ATA_Wait_NBSY-Timeout time is longer (8sec.)
	rcall	DEV_GetErrorFlags_CMD
	cpi	param1,0x00	; check for timeout error
	breq	DEV_DeviceDetection_Seek
	rjmp	DEV_DeviceDetection_End	; no device availabel
		
DEV_DeviceDetection_Seek:
        ldi     temp1,ATA_IDDev		 
        sts     ATA_WACommand,temp1	; write ATA_IDDev command
	
	rcall	ATA_Wait_NBSY
	lds	temp1,DEV_RAStatus	; read DEV_RAStatus	 
		ifdef	dev_debug_high
	rcall	DEV_Read_Taskfile
	rcall	DEV_Print_Taskfile	
		endif
	sbrc	temp1,DEV_ERR
	rjmp	DEV_DeviceDetection_Try_ATAPI	; if an error occured
	rcall	DEV_GetErrorFlags_CMD	; I could also have a timeout error
	cpi	param1,0x00
	brne	DEV_DeviceDetection_Try_ATAPI_JumpBoard	; if an error occured
	
DEV_DeviceDetection_Try_ATA:	; if the command was executed
	rcall	ATA_Wait_DRDY	; check DRDY=1 because ATA devices should support this
	
	SET_ZPOINTER	DEV_Identify_Data
	rcall	ATA_Read_Buffer_Once
	lds	temp1,DEV_RAStatus	; read DEV_RAStatus	 
	sbrc	temp1,DEV_ERR
	rjmp	DEV_DeviceDetection_Error
	rcall	DEV_GetErrorFlags_CMD	; I could also have a timeout error
	cpi	param1,0x00
	brne	DEV_DeviceDetection_Error_Jump
	
DEV_DeviceDetection_ATA_Found:	
	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected	; Master or Slave 
	rjmp	DEV_DeviceDetection_ATA_Found_Master
;	rjmp	DEV_DeviceDetection_ATA_Found_Slave
		
DEV_DeviceDetection_ATA_Found_Slave:	; if it was the Slave
	sbr	temp1,(1<<DEV_SlaveAvailable)	; Slave is available
	sbr	temp1,(1<<DEV_SlaveTyp)		; and it's a ATA device
	sts	DEV_StatusFlags,temp1

	lds	temp1,DEV_SlaveDrvStatusFlags
	sbr	temp1,(1<<DEV_LBA_Mode)		; set LBA-Mode
	; ACHTUNG! CHS Mode is not implemented yet
	sts	DEV_SlaveDrvStatusFlags,temp1
	
;	clr	param1	
;	sts	DEV_SlaveErrorFlags,param1		; clear error flags
	rjmp	DEV_DeviceDetection_CheckNext
	
DEV_DeviceDetection_Try_ATAPI_JumpBoard:		
	rjmp	DEV_DeviceDetection_Try_ATAPI

DEV_DeviceDetection_Error_Jump:	
	rjmp	DEV_DeviceDetection_Error	
	
DEV_DeviceDetection_ATA_Found_Master:	; if it was the Master
	sbr	temp1,(1<<DEV_MasterAvailable)	; Master is  is available
	sbr	temp1,(1<<DEV_MasterTyp)	; and it's a ATA device
	sts	DEV_StatusFlags,temp1
	
	lds	temp1,DEV_MasterDrvStatusFlags
	sbr	temp1,(1<<DEV_LBA_Mode)		; set LBA Mode
	; ACHTUNG! CHS Mode is not implemented yet
	sts	DEV_MasterDrvStatusFlags,temp1
	
;	clr	param1	
;	sts	DEV_MasterErrorFlags,param1		; clear error flags
	rjmp	DEV_DeviceDetection_CheckNext

		
DEV_DeviceDetection_Error:
	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected	; Master or Slave 
	rjmp	DEV_DeviceDetection_Error_Master
;	rjmp	DEV_DeviceDetection_Error_Slave
	
DEV_DeviceDetection_Error_Slave:	; if it was the Slave
	cbr	temp1,(1<<DEV_SlaveAvailable)	; Slave is unavailable
	sts	DEV_StatusFlags,temp1
	rjmp	DEV_DeviceDetection_CheckNext
	
DEV_DeviceDetection_Error_Master:	; if it was the Master
	cbr	temp1,(1<<DEV_MasterAvailable)	; Master is unavailable
	sts	DEV_StatusFlags,temp1
	rjmp	DEV_DeviceDetection_CheckNext
		
		
DEV_DeviceDetection_Try_ATAPI:		; check the ATAPI signature
	lds	temp1,ATAPI_RWACylHigh
	cpi	temp1,0xEB
	brne	DEV_DeviceDetection_Error	; oops, unkown device typ
	lds	temp1,ATAPI_RWACylLow
	cpi	temp1,0x14
	brne	DEV_DeviceDetection_Error	; oops, unkown device typ	

DEV_DeviceDetection_ATAPI_Found:
	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected	; Master or Slave 
	rjmp	DEV_DeviceDetection_ATAPI_Found_Master
;	rjmp	DEV_DeviceDetection_ATAPI_Found_Slave
	
DEV_DeviceDetection_ATAPI_Found_Slave:	; if it was the Slave	
	sbr	temp1,(1<<DEV_SlaveAvailable)	; Slave is available
	cbr	temp1,(1<<DEV_SlaveTyp)		; and it's a ATAPI device
	sts	DEV_StatusFlags,temp1

	lds	temp1,DEV_SlaveDrvStatusFlags
	cbr	temp1,(1<<DEV_DisabledInt)	; enable Interrupts
	; ACHTUNG! Disabled Interrupt Mode is not implemented yet
	sts	DEV_SlaveDrvStatusFlags,temp1

	ldi	param1,0x40	; fake a retry error
	sts	DEV_SlaveErrorFlags,param1
	rcall	ATAPI_CheckCondition_CMD	; I need an idle bus
	
	rjmp	DEV_DeviceDetection_CheckNext

DEV_DeviceDetection_ATAPI_Found_Master:	; if it was the Master	
	sbr	temp1,(1<<DEV_MasterAvailable)	; Master is  is available
	cbr	temp1,(1<<DEV_MasterTyp)	; and it's a ATAPI device
	sts	DEV_StatusFlags,temp1
	
	lds	temp1,DEV_MasterDrvStatusFlags
	cbr	temp1,(1<<DEV_DisabledInt)	; enable Interrupts
	; ACHTUNG! Disabled Interrupt Mode is not implemented yet
	sts	DEV_MasterDrvStatusFlags,temp1
	
	ldi	param1,0x40	; fake a retry error
	sts	DEV_MasterErrorFlags,param1
	rcall	ATAPI_CheckCondition_CMD	; I need an idle bus
	
;	rjmp	DEV_DeviceDetection_CheckNext

	
DEV_DeviceDetection_CheckNext:
	lds	temp1,DEV_StatusFlags
	sbrc	temp1,DEV_DrvSelected	; Master or Slave 
	rjmp	DEV_DeviceDetection_End ; both devices are checked 

	lds	temp1,DEV_StatusFlags
	sbr	temp1,(1<<DEV_DrvSelected)	; I have to check the Slave too
	sts	DEV_StatusFlags,temp1
	ldi	temp1,0x10
	sts	DEV_RWADrvSel,temp1	; write Device/Head reg. to select device
	rjmp	DEV_DeviceDetection_Seek
	
DEV_DeviceDetection_End:
	pop	temp1
	pop	param1
	ret
	
;***********************************************************************
;* FUNCTION	: DEV_HardwareReset_CMD
;*              
;* CREATED	: 19/12/2000 by Marc Dressler
;*              
;* LAST CHANGED : 29/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Send hardware-reset to ide-port and set global DEV-Flags
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*		  DEV_xxxDrvStatusFlags -	updated
;*		  DEV_StatusFlags	-	updated
;*
;***********************************************************************
DEV_HardwareReset_CMD:
	push	param1
	push	param2
	
  ifdef	DEV_HardwareReset_ID
	DEBUG_MSGID DEV_HardwareReset_ID
  endif

	cbi	DEV_HWResetPort, DEV_HWResetPin	; set HWResetPin to low  

	; init Timer/Counter1
	ldi	param1,hi(DEV_TIME_HWReset)	; set CompareA (see devdef.inc)
	ldi	param2,lo(DEV_TIME_HWReset)
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	DEV_Source_HWReset	; start Timer/Counter1 (see devdef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand

	clr	param1
	sts	DEV_MasterErrorFlags,param1	; clear old errors
	sts	DEV_SlaveErrorFlags,param1	; clear old errors

	lds	param1,DEV_MasterDrvStatusFlags
	sbr	param1,(1<<DEV_Medium_Changed)
	sts	DEV_MasterDrvStatusFlags,param1 ; Medium has changed

	lds	param1,DEV_SlaveDrvStatusFlags
	sbr	param1,(1<<DEV_Medium_Changed)
	sts	DEV_SlaveDrvStatusFlags,param1  ; Medium has changed
		
	clr	param1
	sts	DEV_StatusFlags,param1		; disable all devices
	
DEV_HardwareReset_CMD_Loop:
	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
	rjmp    DEV_HardwareReset_CMD_Loop
;        rjmp	DEV_HardwareReset_CMD_End

DEV_HardwareReset_CMD_End:		
	sbi	DEV_HWResetPort, DEV_HWResetPin	; set HWResetPin to high

	pop	param2
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: DEV_SoftwareReset_CMD
;*
;* CREATED	: 14/07/2000 by Jens Funke
;*
;* LAST CHANGED : 12/08/2000 by Jens Funke
;*
;* DESCRIPTION  : Force a Software Reset
;*
;* INPUT        : -
;*
;* OUTPUT       : DEV_xxxErrorFlags	-	cleared
;*
;***********************************************************************
DEV_SoftwareReset_CMD:
	push	param1
	push	param2

  ifdef	DEV_SoftwareReset_ID
	DEBUG_MSGID DEV_SoftwareReset_ID
  endif

	clr	param1				; clear all errors
	rcall	DEV_SetErrorFlags_CMD

	rcall	DEV_GetDrvStatusFlags_CMD	; set SRST in ATAPI_WADevCtr
	andi	param1,(1<<DEV_DisabledInt)	; but don't change nIEN
	sbr	param1,(1<<DEV_SRST)
	sts	ATAPI_WADevCtr,param1
	; Wait for 3 msec.
	; init Timer/Counter1
	ldi	param1,hi(DEV_TIME_SWReset)		; set CompareA to DEV_TIME_SWReset delay
	ldi	param2,lo(DEV_TIME_SWReset)		; see devdef.inc
	rcall	TIMER1_Write_OCR1A

        clr	param1			; clear Timer/Counter1
	clr	param2
	rcall	TIMER1_Write_TCNT
	rcall	DEV_Source_SWReset	; start Timer/Counter1 (see devdef.inc)
	rcall	TIMER1_Clear_OCF1A	; Clear OCF1A by hand

DEV_SoftwareReset_CMD_Loop:
	in	param1,TIFR		; check Timer Overflow of Timer/Counter1
	sbrs	param1,OCF1A
        rjmp	DEV_SoftwareReset_CMD_Loop

	rcall	DEV_GetDrvStatusFlags_CMD	; clear SRST in ATAPI_WADevCtl
	andi	param1,(1<<DEV_DisabledInt)	; but don't change nIEN
	cbr	param1,(1<<DEV_SRST)
	sts	DEV_WADevCtr,param1
	; ACHTUNG! Here we should wait for 31 sec. at least
	rcall	ATA_Wait_NBSY
	; I could also call ATAPI_Wait_NBSY, but ATA_Wait_NBSY waits longer (8sec.)
	;rcall	ATAPI_Wait_NDRQ

	pop	param2
	pop	param1
	ret


		ifdef	dev_command_handler_enabled
;***********************************************************************
;* FUNCTION	: DEV_Command_Handler
;*              
;* CREATED	: 14/01/2001 by Jens Funke
;*              
;* LAST CHANGED : 14/01/2001 by Jens Funke
;*              
;* DESCRIPTION  : -
;*              
;* INPUT        : temp2	-	function see devdef.inc
;*              
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Command_Handler:
	push	temp1
	push	ZL
	push	ZH

	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected		; Master or Slave
	rjmp	DEV_Command_Handler_Master
;	rjmp	DEV_Command_Handler_Slave
	
DEV_Command_Handler_Slave:			; ATAPI or ATA
	sbrs	temp1,DEV_SlaveTyp
	rjmp	DEV_Command_Handler_ATAPI	; if ATAPI then offset+1
	rjmp	DEV_Command_Handler_End
	
DEV_Command_Handler_Master:			; ATAPI or ATA
	sbrs	temp1,DEV_MasterTyp
	rjmp	DEV_Command_Handler_ATAPI	; if ATAPI then offset+1
	rjmp	DEV_Command_Handler_End
	
DEV_Command_Handler_ATAPI:			; if ATAPI
	inc	temp2
	inc	temp2				; temp2 + 2
;	rjmp	DEV_Command_Handler_End		

	; get function address
DEV_Command_Handler_End:
	ldi	ZL,lo(DEV_Commands*2)		; load JumpTable address
	ldi	ZH,hi(DEV_Commands*2)		; load JumpTable address
	add	ZL,temp2			; add offset to JumpTable adress 
	brcc	DEV_Command_Handler_CCarry1
	inc	ZH
DEV_Command_Handler_CCarry1:	
;	push	param1
;	UART_PRINT "\n\rFlash_A1: "		
;	mov	param1,ZH
;	rcall	UART_WriteHex
;	mov	param1,ZL
;	rcall	UART_WriteHex
;	pop	param1
	
	lpm					; load lowbyte from function address
	mov	temp1,r0
	inc	ZL
	brcc	DEV_Command_Handler_CCarry2
	inc	ZH
DEV_Command_Handler_CCarry2:	
;	push	param1
;	UART_PRINT "\n\rFlash_A2: "		
;	mov	param1,ZH
;	rcall	UART_WriteHex
;	mov	param1,ZL
;	rcall	UART_WriteHex
;	pop	param1
	
	lpm					; load highbyte from function address
	mov	ZH,r0	
	mov	ZL,temp1

;	push	param1
;	UART_PRINT "\n\rHandler_A: "		
;	mov	param1,ZH
;	rcall	UART_WriteHex
;	mov	param1,ZL
;	rcall	UART_WriteHex
;	pop	param1

	
	; call JumpTabel function 
	icall		
				
	pop	ZH		; restore the parameter
	pop	ZL
	pop	temp1
	ret
		endif
		


;***********************************************************************
;* FUNCTION	: DEV_Copy_CMD
;*              
;* CREATED	: 20.01.2001 by Marc Dressler
;*              
;* LAST CHANGED : 21.01.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Copy data from Source to Destination
;*              
;* INPUT        : Data structure for copy defined in devdef.inc
;*              
;* OUTPUT       : -
;*
;***********************************************************************
  ifndef DevCopy_disabled
DEV_Copy_CMD:
	push	param1
	push	temp1

DEV_Copy_CMD_InitMedia:
	rcall   DEV_Init_Media_CMD
	rcall   DEV_GetErrorFlags_CMD

	cpi     param1, 0
	breq    DEV_Copy_CMD_InitMediaOk

	rjmp	DEV_Copy_CMD_end
	
DEV_Copy_CMD_InitMediaOk:


DEV_Copy_CMD_Loop:	
			; select source
			
			; read sector

	rcall	DEV_ReadSector_CMD
	rcall   DEV_GetErrorFlags_CMD
	cpi     param1, 0		; read successfull ?
	brne    DEV_Copy_CMD_ReadError
			
	
			; select destination
			
			; write sector

DEV_Copy_CMD_ReadError:

			; increment pointers
			
	ldi	temp1, 1			; sectors read
	lds	param1, DEV_Source_StartSector0	; calculate next sector to write
	add	param1, temp1
	sts	DEV_Source_StartSector0, param1
	ldi	temp1, 0x00
	lds	param1, DEV_Source_StartSector1
	adc	param1, temp1
	sts	DEV_Source_StartSector1, param1
	lds	param1, DEV_Source_StartSector2
	adc	param1, temp1
	sts	DEV_Source_StartSector2, param1
	lds	param1, DEV_Source_StartSector3
	adc	param1, temp1
	sts	DEV_Source_StartSector3, param1
			
	ldi	temp1, 1			; sectors read
	lds	param1, DEV_Dest_StartSector0	; calculate next sector to write
	add	param1, temp1
	sts	DEV_Dest_StartSector0, param1
	ldi	temp1, 0x00
	lds	param1, DEV_Dest_StartSector1
	adc	param1, temp1
	sts	DEV_Dest_StartSector1, param1
	lds	param1, DEV_Dest_StartSector2
	adc	param1, temp1
	sts	DEV_Dest_StartSector2, param1
	lds	param1, DEV_Dest_StartSector3
	adc	param1, temp1
	sts	DEV_Dest_StartSector3, param1
			
	ldi	temp1, 1			; sectors read
	lds	param1, DEV_Source_Len0		; calculate sectorcount to read
	sub	param1, temp1
	sts	DEV_Source_Len0, param1
	ldi	temp1, 0x00
	lds	param1, DEV_Source_Len1
	sbc	param1, temp1
	sts	DEV_Source_Len1, param1
	lds	param1, DEV_Source_Len2
	sbc	param1, temp1
	sts	DEV_Source_Len2, param1
	lds	param1, DEV_Source_Len3
	sbc	param1, temp1
	sts	DEV_Source_Len3, param1

	brcc	DEV_Copy_CMD_end		; check: end of copy area reached ?

	rjmp	DEV_Copy_CMD_Loop		
	
DEV_Copy_CMD_end:
	pop	temp1
	pop	param1
	ret
  endif
		
  ifdef delme
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

				
; DEV_GetErrorFlags_CMD
; DEV_SetErrorFlags_CMD
;	
; DEV_GetDrvStatusFlags_CMD
; DEV_SetDrvStatusFlags_CMD
;			
;***********************************************************************
		restore
	endif

;*
;* $Log: dev.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
