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
;* $Id: xeeprom.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : External EEPROM Include-File
;*  Version         : 1.5
;*
;*  Filename        : xeeprom.inc
;*  Created         : 06.12.2000 by Marc Dressler
;*  Last changed    : 13.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : basic external eeprom functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : i2c.inc
;*
;***********************************************************************

	ifndef	xeeprominc	; prevent multiple binding

xeeprominc	equ	1

	if	mompass=1
	  message "XEEPROM Include-File V1.5 (C)2000 Marc Dressler"
	endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "i2c.inc"

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

XEEPROM_I2CAdr	EQU	0b10100000

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: XEEProm_ReadI2CByte
;*
;* CREATED	: 06.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 06.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : -
;*
;* INPUT        : param2 - msb adress
;*                param3 - lsb adress 
;*
;* OUTPUT       : param1 - result data
;*
;***********************************************************************

XEEProm_ReadI2CByte:
	push	temp1

	mov	temp1, param1

	rcall	I2C_Start

	ldi	param1, XEEProm_I2CAdr	; load device address identifier
	andi	param1, 0b11111110	; bit 0 = 0 : write
	rcall	I2C_WriteByte
	ldi	param1, 0		; 0 in case of error
	brcs	XEEProm_RI2CByE		; branch if error detected

	mov	param1, param2		; load msb adress
	rcall	I2C_WriteByte
	ldi	param1, 0		; 0 in case of error
	brcs	XEEProm_RI2CByE		; branch if error detected

	mov	param1, param3		; load lsb adress
	rcall	I2C_WriteByte
	ldi	param1, 0		; 0 in case of error
	brcs	XEEProm_RI2CByE		; branch if error detected

	rcall	I2C_Start

	ldi	param1, XEEProm_I2CAdr	; load device address identifier
	ori	param1, 0b00000001	; bit 0 = 1 : read
	rcall	I2C_WriteByte
	ldi	param1, 0		; 0 in case of error
	brcs	XEEProm_RI2CByE		; branch if error detected

	rcall	I2C_ReadByte

XEEProm_RI2CByE:
	rcall	I2C_Stop

	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: XEEProm_WriteI2CByte
;*
;* CREATED	: 06.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 06.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : -
;*
;* INPUT        : param1 - data
;*                param2 - MSB adress
;*                param3 - LSB adress
;*
;* OUTPUT       : -
;*
;***********************************************************************

  ifndef STA_eeprom_protect

XEEProm_WriteI2CByte:

	push	temp1

	mov	temp1, param1		; save data

	rcall	I2C_Start

	ldi	param1, XEEProm_I2CAdr	; load device address identifier
	andi	param1, 0b11111110	; bit 0 = 0 : write
	rcall	I2C_WriteByte
	brcs	XEEProm_WI2CByE		; branch if error detected

	mov	param1, param2		; load msb adress
	rcall	I2C_WriteByte
	brcs	XEEProm_WI2CByE		; branch if error detected

	mov	param1, param3		; load lsb adress
	rcall	I2C_WriteByte
	brcs	XEEProm_WI2CByE		; branch if error detected

	mov	param1, temp1		; load data
	rcall	I2C_WriteByte
	brcs	XEEProm_WI2CByE		; branch if error detected	

XEEProm_WI2CByE:
	rcall	I2C_Stop

	rcall	Delay10ms
	rcall	Delay10ms
	
	pop	temp1
	ret

  endif

;***********************************************************************

	endif

;*
;* $Log: xeeprom.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
