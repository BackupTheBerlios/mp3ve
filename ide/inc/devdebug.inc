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
;* $Id: devdebug.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Device Debug Functions
;*  Version     : 1.5
;*				
;*  Filename    : devdebug.inc
;*  Created     : 05/12/2000 by Jens Funke
;*  Last changed: 05/12/2000 by Jens Funke
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
   	ifndef  devdebuginc       ; prevent multiple binding
devdebuginc       equ     1

		save
                listing off

                if       mompass=1
                 message "Device Debug Functions (C) 2000 by Jens Funke"
                endif

	SEGMENT DATA
;***********************************************************************
;* Taskfile
;***********************************************************************
DEV_DataLow		res	1
DEV_DataHigh		res	1
DEV_Error		res	1
DEV_Interrupt		res	1
DEV_CylinderLow		res	1
DEV_CylinderHigh	res	1
DEV_DrvSelect		res	1
DEV_Status		res	1
DEV_Feature		res	1
DEV_Command		res	1
DEV_AltStatus		res	1
DEV_DevControl		res	1

	
	SEGMENT CODE
;***********************************************************************
;* FUNCTION	: DEV_Wait_Key
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 23.04.2000 by Jens Funke
;*
;* DESCRIPTION  : Wait for pressing a Key
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Wait_Key:
	push	param1
	rcall	UART_Read
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_Status
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read StatusByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_Status:
	push	param3
	push	ZL
	push	ZH
	UART_PRINT "\n\rDEV_Status: "
        lds     param3,DEV_RAStatus      ;read StatusByte
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_Error
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read ErrorByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_Error:
	push	param3
	push	ZL
	push	ZH
	UART_PRINT "\n\rDEV_Error: "
        lds     param3,DEV_RAError      ;read ErrorByte
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: ATAPI_Print_Interrupt
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read InterruptByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
;ATAPI_Print_Interrupt:
;	push	param3
;	push	ZL
;	push	ZH
;        UART_PRINT "\n\rATAPI_Interrupt: "
;        lds     param3,ATAPI_Interrupt      ;read InterruptByte
;	SET_ZPOINTER	DATA1		;Macro
;	rcall	UART_Print_As_Bin
;	pop	ZH
;	pop	ZL
;	pop	param3
;	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_CylinderLow
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read CylinderLowByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_CylinderLow:
	push	param3
	push	ZL
	push	ZH
        UART_PRINT "\n\rDEV_CylinderLow: "
        lds     param3,DEV_CylinderLow      ;read CylinderLowByte
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_CylinderHigh
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read CylinderHighByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_CylinderHigh:
	push	param3
	push	ZL
	push	ZH
        UART_PRINT "\n\rDEV_CylinderHigh: "
        lds     param3,DEV_CylinderHigh      ;read CylinderHighByte
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_DrvSelect
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read DrvSelectByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_DrvSelect:
	push	param3
	push	ZL
	push	ZH
        UART_PRINT "\n\rDEV_DriveSelect: "
        lds     param3,DEV_DrvSelect      ;read DrvSelectByte
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_AltStatus
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read AltStatusByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_AltStatus:
	push	param3
	push	ZL
	push	ZH
        UART_PRINT "\n\rDEV_AlternateStatus: "
        lds     param3,DEV_AltStatus      ;read AltStatusByte
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_DevControl   ACHTUNG!L"oschen, ist altStatus!
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read DevControlByte and Print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_DevControl:
	push	param3
	push	ZL
	push	ZH
        UART_PRINT "\n\rDEV_DeviceControl: "
        lds     param3,DEV_DevControl      ;read DevControlByte
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_PORTD
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : read PORTD and print it to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_PORTD:
	push	param3
	push	ZL
	push	ZH
	UART_PRINT "\n\rPORTD: "
	in	param3,pind		; read PORTD
	SET_ZPOINTER	DATA1		;Macro
	rcall	UART_Print_As_Bin
	pop	ZH
	pop	ZL
	pop	param3
	ret

;***********************************************************************
;* FUNCTION	: DEV_Print_Taskfile
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 23.04.2000 by Jens Funke
;*
;* DESCRIPTION  : Print the Taskfile to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Print_Taskfile:
	rcall	DEV_Print_Error
;	rcall	ATAPI_Print_Interrupt
	rcall	DEV_Print_CylinderLow
	rcall	DEV_Print_CylinderHigh
	rcall	DEV_Print_DrvSelect
	rcall	DEV_Print_Status
	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_Taskfile
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 23.04.2000 by Jens Funke
;*
;* DESCRIPTION  : Read Taskfile
;*
;* INPUT        : IDE-Interface
;*
;* OUTPUT       : Variables in RAM
;*
;***********************************************************************
DEV_Read_Taskfile:
	push	temp1
	lds	temp1,DEV_RAError
	sts	DEV_Error,temp1
;	lds	temp1,ATAPI_RWAInterrupt
;	sts	ATAPI_Interrupt,temp1
	lds	temp1,DEV_RWACylLow
	sts	DEV_CylinderLow,temp1
	lds	temp1,DEV_RWACylHigh
	sts	DEV_CylinderHigh,temp1
	lds	temp1,DEV_RWADrvSel
	sts	DEV_DrvSelect,temp1
	lds	temp1,DEV_RAStatus
	sts	DEV_Status,temp1
	lds	temp1,DEV_RAAltStatus
	sts	DEV_AltStatus,temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Write_Taskfile
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 23.04.2000 by Jens Funke
;*
;* DESCRIPTION  : Write Taskfile
;*
;* INPUT        : Variables in RAM
;*
;* OUTPUT       : IDE-Interface
;*
;***********************************************************************
DEV_Write_Taskfile:
	push	temp1
	lds	temp1,DEV_Feature
	sts	DEV_WAFeature,temp1
	lds	temp1,DEV_CylinderLow
	sts	DEV_RWACylLow,temp1
	lds	temp1,DEV_CylinderHigh
	sts	DEV_RWACylHigh,temp1
	lds	temp1,DEV_DrvSelect
	sts	DEV_RWADrvSel,temp1
	lds	temp1,DEV_Command
	sts	DEV_WACommand,temp1
	lds	temp1,DEV_DevControl
	sts	DEV_WADevCtr,temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_Error
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 04.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the DEV_Error Register
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Read_Error:
	push	temp1
	lds	temp1,DEV_RAError
	sts	DEV_Error,temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_Interrupt
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 04.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the DEV_Interrupt Register
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
;DEV_Read_Interrupt:
;	push	temp1
;	lds	temp1,DEV_RWAInterrupt
;	sts	DEV_Interrupt,temp1
;	pop	temp1
;	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_CylinderLow
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 04.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the DEV_CylinderLow Register
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Read_CylinderLow:
	push	temp1
	lds	temp1,DEV_RWACylLow
	sts	DEV_CylinderLow,temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_CylinderHigh
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 04.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the DEV_CylinderHigh Register
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Read_CylinderHigh:
	push	temp1
	lds	temp1,DEV_RWACylHigh
	sts	DEV_CylinderHigh,temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_DrvSelect
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 04.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the DEV_DrvSelect Register
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Read_DrvSelect:
	push	temp1
	lds	temp1,DEV_RWADrvSel
	sts	DEV_DrvSelect,temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_Status
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 04.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the DEV_Status Register
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Read_Status:
	push	temp1
	lds	temp1,DEV_RAStatus
	sts	DEV_Status,temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: DEV_Read_AltStatus
;*
;* CREATED	: 04.05.2000 by Jens Funke
;*
;* LAST CHANGED : 04.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the DEV_AltStatus Register
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
DEV_Read_AltStatus:
	push	temp1
	lds	temp1,DEV_RAAltStatus
	sts	DEV_AltStatus,temp1
	pop	temp1
	ret


;***********************************************************************
		restore
	endif

;*
;* $Log: devdebug.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
