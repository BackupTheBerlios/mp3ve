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
;* $Id: mp3ser.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	    : MP3 Service Functions
;*  Version         : 1.5
;*				
;*  Filename        : mp3ser.inc
;*  Created         : 03/12/2000 by Jens Funke
;*  Last changed    : 02.03.2001 by Marc Dressler
;*	
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description	    : -
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : dev.inc
;*		      atapi.inc
;*		      ata.inc
;*
;***********************************************************************
	ifndef mp3serinc
mp3serinc	=	1

		save

                if       mompass=1
                 message "MP3 Service V1.5 (C) 2000 by Jens Funke"
                endif

MP3_MaxReadErrors equ	0x01 	; counter for max. read tries when error occoured
MP3_SecCount	equ	0x01	; number of sectors to read in one packet command
				; 0x08 = 1sec (128kbit)

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: MP3_ReadDevice
;*
;* CREATED	: 17.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 02.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : Read a single byte from the selected device
;*
;* INPUT        : -
;*
;* OUTPUT       : param1 - next data-byte from IDE-Cache
;*		  MP3_Flags - Bit MP3_Finvalide
;*		  Error - Bit Error_fMP3
;*
;***********************************************************************

MP3_ReadDevice:
	cbr	MP3_Flags, (1<<MP3_Finvalide)	; no error 
	
	sbrc	MP3_Flags, MP3_ftmpdata
	rjmp	MP3_ReadDeviceByteCache		; read from Byte-Cache

	push	temp2				; error counter
	ldi	temp2, MP3_MaxReadErrors	; load max. read-tries
	
MP3_ReadDeviceByteLoop:
	sbrc	MP3_Flags, MP3_Finvalide
	rjmp	MP3_ReadDeviceByteEnd

	lds	param1, DEV_RAStatus

	sbrc	param1, DEV_BSY
	rjmp	MP3_ReadDeviceByteLoop

  ifndef mp3_NoReadErrorCheck
	sbrc	param1, DEV_ERR
	rjmp	MP3_ReadDeviceByteError
  endif
  
	sbrs	param1, DEV_DRQ
	rjmp	MP3_ReadDeviceByteNoData
	
MP3_ReadDeviceByteData:
	sbr	MP3_Flags, (1<<MP3_ftmpdata)

	lds	param1, DEV_RWADataLow		; read IDE-Cache - first byte
	lds	MP3_Data, DEV_RWADataHigh	; read IDE-Cache - next byte

MP3_ReadDeviceByteEnd:	
	pop	temp2
	ret
	
MP3_ReadDeviceByteCache:
	cbr	MP3_Flags, (1<<MP3_ftmpdata)
	mov	param1, MP3_Data
	ret

MP3_ReadDeviceByteError:
;  ifdef	mp3_debug
  ifdef	mp3_retrigger_debug
	push	param1
	mov 	param1, temp2
	rcall	UART_WriteHEX
	rcall	UART_WriteCRLF
	pop	param1
  endif
	
	rcall	MP3_ErrorFlagsUpdate
	
	dec	temp2				; decrement read-tries
	brne	MP3_ReadDeviceByte_Init		; 0 reached ?

; max count of read-tries reached
	lds	temp1, Error			; set Error_fMP3 in Error
	sbr	temp1, (1<<Error_fMP3)
	sts	Error, temp1
	
	sbr	MP3_Flags, (1<<MP3_Finvalide)	; data invalide 	
	
	rjmp	MP3_ReadDeviceByteSkipSector

MP3_ReadDeviceByteNoData:
	rcall	MP3_ErrorFlagsUpdate

MP3_ReadDeviceByte_Init:			; send new job
	sbrs	MP3_Flags1, MP3_fsoftstop	; check if MP3 SoftStop activated
	rjmp	MP3_ReadDeviceByte_Go		; and send new job
  
	sbr	MP3_Flags, (1<<MP3_Finvalide)	; data invalide
	rjmp	MP3_ReadDeviceByteEnd

MP3_ReadDeviceByte_Go:
	rcall	MP3_Read_Command		; send read command to the device
	
	lds	param1, DEV_RAStatus		; check device error register
	sbrc	param1, DEV_ERR
	rjmp	MP3_ReadDeviceByteError		; something went wrong!
	
MP3_ReadDeviceByteSkipSector:
	push	temp1
	
	ldi	temp1, MP3_SecCount		; sectors read
	lds	param1, MP3_Sector1		; calculate next sector to read
	add	param1, temp1
	sts	MP3_Sector1, param1
	ldi	temp1, 0x00
	lds	param1, MP3_Sector2
	adc	param1, temp1
	sts	MP3_Sector2, param1
	lds	param1, MP3_Sector3
	adc	param1, temp1
	sts	MP3_Sector3, param1
	lds	param1, MP3_Sector4
	adc	param1, temp1
	sts	MP3_Sector4, param1

	ldi	temp1, MP3_SecCount		; sectors read
	lds	param1, MP3_SectorCount1	; calculate sectorcount to read
	sub	param1, temp1
	sts	MP3_SectorCount1, param1
	ldi	temp1, 0x00
	lds	param1, MP3_SectorCount2
	sbc	param1, temp1
	sts	MP3_SectorCount2, param1
	lds	param1, MP3_SectorCount3
	sbc	param1, temp1
	sts	MP3_SectorCount3, param1
	lds	param1, MP3_SectorCount4
	sbc	param1, temp1
	sts	MP3_SectorCount4, param1

	brcc	MP3_ReadDeviceByteNoSkip	; check: end of file reached ?
	rcall	MP3_AutoSkip

MP3_ReadDeviceByteNoSkip:
	pop	temp1
	rjmp	MP3_ReadDeviceByteLoop

 ifdef fasttest ; xxx test version
MP3_FastErrorFlagsUpdate:
	push	param1
	lds	param1, DEV_RAStatus		; check device error register
	sbrs	param1, DEV_ERR
	rjmp	MP3_FastErrorFlagsUpdateEnd

	rcall	ATAPI_ReqSense_CMD
	rcall	ATAPI_ClearBuffer

MP3_FastErrorFlagsUpdateEnd:
	pop	param1
	ret
  endif

;***********************************************************************
;* FUNCTION	: MP3_ErrorFlagsUpdate
;*              
;* CREATED	: 03/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 16.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : Updates ErrorFlags of the selected device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : DEV_xxxErrorFlags	-	updated
;*
;***********************************************************************
MP3_ErrorFlagsUpdate:
	push	temp1

	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected		; Master or Slave
	rjmp	MP3_ErrorFlagsUpdate_Master
;	rjmp	MP3_ErrorFlagsUpdate_Slave
	
MP3_ErrorFlagsUpdate_Slave:			; ATAPI or ATA
	sbrs	temp1,DEV_SlaveTyp
	rjmp	MP3_ErrorFlagsUpdate_ATAPI
	rjmp	MP3_ErrorFlagsUpdate_ATA
	
MP3_ErrorFlagsUpdate_Master:			; ATAPI or ATA
	sbrs	temp1,DEV_MasterTyp
	rjmp	MP3_ErrorFlagsUpdate_ATAPI
;	rjmp	MP3_ErrorFlagsUpdate_ATA
	
MP3_ErrorFlagsUpdate_ATA:			; if ATA
	rcall	ATA_ErrorFlagsUpdate
	rjmp	MP3_ErrorFlagsUpdate_End		
	
MP3_ErrorFlagsUpdate_ATAPI:			; if ATAPI
	rcall	ATAPI_CheckCondition_CMD
;	rjmp	MP3_ErrorFlagsUpdate_End		
		
MP3_ErrorFlagsUpdate_End:
	pop	temp1
	ret
	
;***********************************************************************
;* FUNCTION	: MP3_Read_Command
;*              
;* CREATED	: 03/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 02.01.2001 by Marc Dressler
;*              
;* DESCRIPTION  : Send a Read Command to the selected device
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
MP3_Read_Command:
  ifdef	mp3_debug
;  ifdef	mp3_retrigger_debug
	push	param1
	lds	param1, MP3_Sector4
	rcall	UART_WriteHex
	lds	param1, MP3_Sector3
	rcall	UART_WriteHex
	lds	param1, MP3_Sector2
	rcall	UART_WriteHex
	lds	param1, MP3_Sector1
	rcall	UART_WriteHex

	ldi	param1, '-'
	rcall	UART_Write

	lds	param1, MP3_SectorCount4
	rcall	UART_WriteHex
	lds	param1, MP3_SectorCount3
	rcall	UART_WriteHex
	lds	param1, MP3_SectorCount2
	rcall	UART_WriteHex
	lds	param1, MP3_SectorCount1
	rcall	UART_WriteHex
	rcall	UART_WriteCRLF
	pop	param1
  endif

	push	temp1

	lds	temp1,DEV_StatusFlags
	sbrs	temp1,DEV_DrvSelected		; Master or Slave
	rjmp	MP3_Read_Command_Master
;	rjmp	MP3_Read_Command_Slave
	
MP3_Read_Command_Slave:				; ATAPI or ATA
	sbrs	temp1,DEV_SlaveTyp
	rjmp	MP3_Read_Command_ATAPI
	rjmp	MP3_Read_Command_ATA
	
MP3_Read_Command_Master:			; ATAPI or ATA
	sbrs	temp1,DEV_MasterTyp
	rjmp	MP3_Read_Command_ATAPI
;	rjmp	MP3_Read_Command_ATA
	
MP3_Read_Command_ATA:				; if ATA
	rcall	MP3_ATA_Read_Command
	rjmp	MP3_Read_Command_End		
	
MP3_Read_Command_ATAPI:				; if ATAPI
	rcall	MP3_ATAPI_Read10_Command
;	rjmp	MP3_Read_Command_End		
		
MP3_Read_Command_End:
	pop	temp1
	ret
	
;***********************************************************************
;* FUNCTION	: MP3_ATAPI_Read10_Command
;*              
;* CREATED	: 03/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 19.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : Send a Read Command to the ATAPI Drive
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
MP3_ATAPI_Read10_Command:
	rcall	ATAPI_Wait_NBSY

        lds	param1, DEV_StatusFlags		; select drive
	andi	param1, (1<<DEV_DrvSelected)
	sts	ATAPI_RWADrvSel, param1
        ldi     param1, ATAPI_Packet        	; write Packet Command
        sts     ATAPI_WACommand, param1

	rcall	ATAPI_Wait_NBSY			; It can't be wrong

;The order of HighByte and LowByte is importent
	clr	param1
	sts	ATAPI_RWADataHigh, param1	; Byte1
        ldi	param1, ATAPI_Read10		; Packet Command
	sts	ATAPI_RWADataLow, param1	; Byte0

	lds	param1, MP3_Sector3
	sts	ATAPI_RWADataHigh, param1	; Logical Block Address2
	lds	param1, MP3_Sector4
	sts	ATAPI_RWADataLow, param1	; Logical Block Address3 (MSB)

	lds	param1, MP3_Sector1
	sts	ATAPI_RWADataHigh, param1	; Logical Block Address0 (LSB)
	lds	param1, MP3_Sector2
	sts	ATAPI_RWADataLow, param1	; Logical Block Address1

	clr	param1
	sts	ATAPI_RWADataHigh, param1	; Transfer Length1 (MSB)
	sts	ATAPI_RWADataLow, param1	; Byte6

	sts	ATAPI_RWADataHigh, param1	; Byte9
	ldi	param1, MP3_SecCount
	sts	ATAPI_RWADataLow, param1	; Transfer Length0 (LSB)

	clr	param1
	sts	ATAPI_RWADataHigh, param1	; Byte11
	sts	ATAPI_RWADataLow, param1	; Byte10

  ifdef	 atapi_int_enabled
	rcall	ATAPI_Wait_INTRQ		; wait for INTRQ
  endif
  ifndef atapi_int_enabled
	rcall	ATAPI_Wait_NBSY			; wait for NBSY
  endif

	ret

;***********************************************************************
;* FUNCTION	: MP3_ATA_Read_Command
;*              
;* CREATED	: 03/12/2000 by Jens Funke
;*              
;* LAST CHANGED : 03/12/2000 by Jens Funke
;*              
;* DESCRIPTION  : Send a Read Command to the ATA Drive
;*              
;* INPUT        : -
;*              
;* OUTPUT       : -
;*
;***********************************************************************
MP3_ATA_Read_Command:
	push	param1

	rcall	DEV_GetDrvStatusFlags_CMD
	andi	param1,(1<<DEV_LBA_Mode)	; only bit 6
	
	lds	temp1,DEV_StatusFlags
	andi	temp1,(1<<DEV_DrvSelected)	; only bit 4
	
	or	temp1,param1	; calculate high nibble of Device/Head reg.

	lds	param4,MP3_Sector1		; MP3_Sector1 (LSB)
	lds	param3,MP3_Sector2
	lds	param2,MP3_Sector3
	lds	param1,MP3_Sector4		; MP3_Sector4 (MSB)
	rcall	ATA_ATAPISector2ATASector	; calculate the address
	
	andi	param1,(0x0F)			; i need only the lower nibble
	or	param1,temp1		; now create the Device/Head reg.
	 
	rcall   ATA_Wait_NBSY
			
	sts	ATA_RWADevHead,param1		; write Device/Head reg.
	sts	ATA_RWACylHigh,param2		; write Cylinder reg.
	sts	ATA_RWACylLow,param3
	sts	ATA_RWASecNo,param4		; write Sector Number reg
	
	ldi	param1,(MP3_SecCount*4)		; i need 2048 * MP3_SecCount byte 	 
	sts	ATA_RWASecCount,param1		; write Sector Count reg.
	
        ldi     temp1,ATA_ReadSecLoop		
        sts     ATA_WACommand,temp1		; write ReadCommand 	
	rcall   ATA_Wait_NBSY

	pop	param1
	ret
	
;***********************************************************************
		restore
	endif

;*
;* $Log: mp3ser.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
