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
;* $Id: debug.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : Debug Include-File
;*  Version         : 1.5
;*
;*  Filename        : debug.inc
;*  Created         : 26.12.2000 by Marc Dressler
;*  Last changed    : 10.02.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : debug modul
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : rpc.inc
;*
;***********************************************************************

	ifndef	debuginc	; prevent multiple binding

debuginc	equ	1

		if	mompass=1
                 message "Debug Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "debugdef.inc"	
	INCLUDE "rpctab.inc"	
	
;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************


;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

	SEGMENT DATA

Debug_Data:
Debug_DataID:	res	1
Debug_DataHi:	res	1
Debug_DataLo:	res	1
Debug_DataEnd:

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: Debug_Init
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 26.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : init debug system
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
Debug_Init:
	push	temp1
	ldi	temp1, 0
	sts	Debug_DataID, temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: Debug_Send
;*
;* CREATED	: 21.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 21.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : send debug information via rpc
;*
;* INPUT        : param2 - lo (debug-id)
;*                param3 - hi (debug-id)
;*
;* OUTPUT       : -
;*
;***********************************************************************

Debug_Send:
	push	zl
	push	zh
	
	sts	Debug_DataHi, param3
	sts	Debug_DataLo, param2
	ldi	param2, RPC_CMD_Debug_Info
	ldi	param3, (Debug_DataEnd-Debug_Data)

	ldi	zl, lo(Debug_Data)
	ldi	zh, hi(Debug_Data)
	rcall	RPC_TransmitData
	
	pop	zh
	pop	zl
	ret
	
;***********************************************************************
;* FUNCTION	: Debug_Info
;*
;* CREATED	: 26.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 19.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : send debug information to uart
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
Debug_Info:
	push	param1

	ldi	param1, RPC_CMD_Debug_Info
	sts	RPC_TxID, param1
	
; System-Flags
	lds	param1, Error
	sts	RPC_TxData0, param1
		
;	lds	param1, TOC_ErrorFlags
;	sts	RPC_TxData1, param1

;	lds	param1, DEV_ErrorFlags
;	sts	RPC_TxData2, param1
	
; MP3-Flags
	lds	param1, MP3_ErrorFlags
	sts	RPC_TxData3, param1

	mov	param1, MP3_Flags
	sts	RPC_TxData4, param1
	
	mov	param1, MP3_Flags1
	sts	RPC_TxData5, param1

; DEVICE-Flags	
	lds	param1, DEV_StatusFlags
	sts	RPC_TxData6, param1

	lds	param1, DEV_MasterErrorFlags
	sts	RPC_TxData7, param1

	lds	param1, DEV_SlaveErrorFlags
	sts	RPC_TxData8, param1

	ldi	param1, 9
	sts	RPC_TxLen, param1 ; send 10 bytes (RPC_TxData0 to RPC_TxData9) 
	
	pop	param1
	
	rcall	RPC_Transmit
	ret
	
;***********************************************************************

	endif

;*
;* $Log: debug.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
