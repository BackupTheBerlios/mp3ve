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
;* $Id: sta.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : STA Include-File
;*  Version         : 1.5
;*
;*  Filename        : sta.inc
;*  Created         : 26.04.2000 by Marc Dressler
;*  Last changed    : 05.02.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : basic sta functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : i2c.inc
;*		      mp3.inc
;*		      xeeprom.inc
;*
;***********************************************************************

	ifndef	stainc	; prevent multiple binding

stainc	equ	1

	if	mompass=1
	  message "STA Include-File V1.5 (C)2000 Marc Dressler"
	endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "i2c.inc"
	INCLUDE "mp3.inc"
	INCLUDE "xeeprom.inc"

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

STA_I2CAdr	EQU	0b10000111

        SEGMENT CODE


;***********************************************************************
;* FUNCTION	: STA_ReadI2CByte
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 13.11.1999 by Marc Dressler
;*
;* DESCRIPTION  : -
;*
;* INPUT        : param1 - subadress
;*
;* OUTPUT       : param1 - result
;*
;***********************************************************************

STA_ReadI2CByte:
	push	temp1

	mov	temp1, param1

	rcall	I2C_Start

	ldi	param1, STA_I2CAdr	;load STA013 device address identifier
	andi	param1, 0b11111110	;bit 0 = 0 : write
	rcall	I2C_WriteByte
	ldi	param1, 0		;0 in case of error
	brcs	STA_RI2CByE		;branch if error detected

	mov	param1, temp1		;load subaddress
	rcall	I2C_WriteByte
	ldi	param1, 0		;0 in case of error
	brcs	STA_RI2CByE		;branch if error detected

	rcall	I2C_Start

	ldi	param1, STA_I2CAdr	;load STA013 device address identifier
	ori	param1, 0b00000001	;bit 0 = 1 : read
	rcall	I2C_WriteByte
	ldi	param1, 0		;0 in case of error
	brcs	STA_RI2CByE		;branch if error detected

	rcall	I2C_ReadByte

STA_RI2CByE:
	rcall	I2C_Stop

	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: STA_WriteI2CByte
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 13.11.1999 by Marc Dressler
;*
;* DESCRIPTION  : -
;*
;* INPUT        : param1 - subadress
;*                param2 - data
;*
;* OUTPUT       : -
;*
;***********************************************************************

STA_WriteI2CByte:
	push	temp1

	mov	temp1,param1

	rcall	I2C_Start

	ldi	param1, STA_I2CAdr	;load STA013 device address identifier
	andi	param1, 0b11111110	;bit 0 = 0 : write
	rcall	I2C_WriteByte
	brcs	STA_WI2CByE		;branch if error detected

	mov	param1, temp1		;load subaddress
	rcall	I2C_WriteByte
	brcs	STA_WI2CByE		;branch if error detected

	mov	param1, param2		;load data
	rcall	I2C_WriteByte
	brcs	STA_WI2CByE		;branch if error detected

STA_WI2CByE:
	rcall	I2C_Stop

	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: STA_Init
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 05.02.2001 by Marc Dressler
;*
;* DESCRIPTION  : -
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

STA_Init:
	
  ifdef	sta_debug
	ldi	zl, lo(TxSTAInit*2)
	ldi	zh, hi(TxSTAInit*2)
	rcall	UART_WriteTxt

	ldi	zl, lo(TxVersion*2)
	ldi	zh, hi(TxVersion*2)
	rcall	UART_WriteTxt
  endif

	ldi	param1, 0x10		;SOFT_RESET
	ldi	param2, 1		;
	rcall	STA_WriteI2CByte
	
  ifdef	sta_debug
	ldi	param1, 0		;version
	rcall	STA_ReadI2CByte		;

	rcall	UART_WriteHex
	rcall	UART_WriteCRLF

	ldi	zl, lo(TxIdent*2)
	ldi	zh, hi(TxIdent*2)
	rcall	UART_WriteTxt
	ldi	param1, 1		;ident
	rcall	STA_ReadI2CByte
	rcall	UART_WriteHex
	rcall	UART_WriteCRLF
  endif

	ldi	param1, 1		;ident
	rcall	STA_ReadI2CByte		;read ident (0xac ?)

	ldi	param2, 0xac
	cpse	param1, param2
	ret				;sta not found 

	ldi	param1, 0x02		;sta found
	sts	MP3_Chip, param1
	
;PCM output interface configuration

	rcall	STA_SendPatch		;send patch from eeprom 

	ldi	param1, 0x54		;PCMDIVIDER
  ifdef cs4338
	ldi	param2, 3		;O_FAC=256 16bit
  endif
  ifdef cs4331
	ldi	param2, 1		;O_FAC=256 32bit	
  endif
	rcall	STA_WriteI2CByte

	ldi	param1, 0x55		;PCMCONF

  ifdef cs4338
	ldi	param2, 0b00110000	;X,MSB first,left padded,I2S,I2S,falling edge,16bit,16bit
  endif
	
  ifdef cs4331
	ldi	param2, 0b00110001	;X,MSB first,left padded,I2S,I2S,falling edge,16bit,18bit
  endif
		
	rcall	STA_WriteI2CByte

;PLL configuration for 14M31818
	ldi	param1, 6		;???
	ldi	param2, 12		;???
	rcall	STA_WriteI2CByte
   
	ldi	param1, 11		;???
	ldi	param2, 3		;???
	rcall	STA_WriteI2CByte

	ldi	param1, 97		;MSFDF
	ldi	param2, 15		;
	rcall	STA_WriteI2CByte

	ldi	param1, 80		;MSFDF-441
	ldi	param2, 16		;
	rcall	STA_WriteI2CByte

	ldi	param1, 101		;PLLFRAC-H
	ldi	param2, 187		;
	rcall	STA_WriteI2CByte
   
	ldi	param1, 82		;PLLFRAC-441-H
	ldi	param2, 103		;
	rcall	STA_WriteI2CByte
   
	ldi	param1, 100		;PLLFRAC-L
	ldi	param2, 58		;
	rcall	STA_WriteI2CByte

	ldi	param1, 81		;PLLFRAC-441-L
	ldi	param2, 119		;
	rcall	STA_WriteI2CByte
   
	ldi	param1, 5		;PLLCTRL
	ldi	param2, 0b10100001	;161
	rcall	STA_WriteI2CByte

;input serial clock polarity configuration

	ldi	param1, 0x0d		;SCLK_POL
;	ldi	param2, 0x00		;rising edge
	ldi	param2, 0x04		;falling edge
	rcall	STA_WriteI2CByte

;data request pin enable

	ldi	param1,	0x18		;DATA_REQ_ENABLE
	ldi	param2,	0b00000100	;enable request signal
	rcall	STA_WriteI2CByte

; data request polarity configuration

	ldi	param1,	0x0c		;REQ_POL
	ldi	param2,	0b00000001	;send when high
	rcall	STA_WriteI2CByte

; run
	ldi	param1,	0x72		;RUN
	ldi	param2,	0x01		;start decoding process
	rcall	STA_WriteI2CByte

  ifdef	sta_debug
	ldi	zl, lo(TxReady*2)
	ldi	zh, hi(TxReady*2)
	rcall	UART_WriteTxt
  endif

	ret
	
;***********************************************************************
;* FUNCTION	: STA_SendPatch
;*
;* CREATED	: 06.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 06.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : read patch from xeeprom and send it to sta
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

STA_SendPatch:
        push    temp1
        push    param1
        push	param2
	push	param3
	push	xl
	push	xh
	
	ldi	xl, 0x00		; lsb xeeprom adress
	ldi	xh, 0x00		; msb xeeprom adress

STA_Ploop:
	mov	param2, xh		; msb adress
	mov	param3, xl		; lsb adress
	rcall	XEEProm_ReadI2CByte	; read data to param1
	mov	temp1, param1		; store data
	adiw    xl, 0x01		; inc xeeprom-pointer

	mov	param2, xh		; msb adress
	mov	param3, xl		; lsb adress
	rcall	XEEProm_ReadI2CByte	; read adress to param1
	adiw    xl, 0x01		; inc xeeprom-pointer
	
        cpi     param1, 0x00		; compare with 0x00
        breq    STA_Pret		; jump, if end reached

	mov	param2, temp1		; load data
	rcall	STA_WriteI2CByte	; write to sta

        rjmp    STA_Ploop		; next data
STA_Pret:
	pop	xh
	pop	xl
	pop	param3
	pop	param2
        pop     param1
        pop     temp1
        ret

;***********************************************************************
;* FUNCTION	: STA_SetVolume
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : set volume
;*
;* INPUT        : param2 - volume
;*
;* OUTPUT       : -
;*
;***********************************************************************

STA_SetVolume:
	push	param1

	ldi	param1,	0x7d		;TONE_ATTEN
	rcall	STA_WriteI2CByte

	pop	param1
	ret

;***********************************************************************

  ifdef	sta_debug

TxSTAInit:
	data	"STA Init\n\r",0
TxVersion:
	data	"STA Version\n\r",0
TxIdent:
	data	"STA Ident\n\r",0
TxReady:
	data	"STA Ready\n\r",0

  endif

;***********************************************************************

	endif

;*
;* $Log: sta.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
