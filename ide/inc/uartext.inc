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
;* $Id: uartext.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : UART Extention Include-File
;*  Version         : 1.5
;*
;*  Filename        : uartext.inc
;*  Created         : 06.05.2000 by Marc Dressler
;*  Last changed    : 13.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : all
;*  Description     : additional uart functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : uart.inc
;*                    convert.inc
;*
;***********************************************************************
	ifndef	uartextinc
uartextinc	=	1

		if	mompass=1
                 message "UART Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************
	INCLUDE	"uart.inc"
	INCLUDE	"convert.inc"
	INCLUDE "macros.mac"
	
  ifdef	include_uartext
;***********************************************************************
;* FUNCTION	: UART_WriteBCD
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 13.11.1999 by Marc Dressler
;*
;* DESCRIPTION  : write value in register 'param1' as bcd-code to UART
;*		!!!!!!!! NOT READY !!!!!!!!!!!!!
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WriteBCD:

        ret

;***********************************************************************
;* FUNCTION	: UART_WriteDEC
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 13.11.1999 by Marc Dressler
;*
;* DESCRIPTION  : write value in register 'param1' as dec-code to UART
;*		!!!!!!!! NOT READY !!!!!!!!!!!!!
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WriteDEC:

        ret

;***********************************************************************
;* FUNCTION     : UART_Read31
;*
;* CREATED      : 04.01.2000 by Jens Funke
;*
;* LAST CHANGED : 05.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Read UART until Enter or 32 character. The last char
;*                must be 0.
;*
;* INPUT        : ZH - HighPointer to SRAM
;*                ZL - LowPointer to SRAM
;*
;* OUTPUT       : ZH - HighPointer to SRAM
;*                ZL - LowPointer to SRAM
;*
;***********************************************************************
UART_Read31:
        push    ZH                  ;HighPointer
        push    ZL                  ;LowPointer
        push    temp1
	push	param1

        ldi     temp1,31
UART_Read31Again:
        rcall   UART_Read

        cpi     param1,LF               ;compare charakter with LF
        breq    UART_Read31Stop         ;branch if equal
        cpi     temp1,0                 ;stop after 31 loops
        breq    UART_Read31Stop
        st      Z+,param1               ;store char from UART
        dec     temp1
        rjmp    UART_Read31Again

UART_Read31Stop:
        ldi     param1,0                ;last charakter must be 0
        st      Z,param1

	pop	param1
        pop     temp1
        pop     ZL
        pop     ZH
        ret

;***********************************************************************
;* FUNCTION     : UART_ReadX
;*
;* CREATED      : 04.01.2000 by Jens Funke
;*
;* LAST CHANGED : 04.01.2000 by Jens Funke
;*
;* DESCRIPTION  : UART_Readx will read x charakters from UART.
;*                This function will return x+1 charakters, because
;*                the last must be 0.
;*
;* INPUT        : ZH     - HighPointer to SRAM
;*                ZL     - LowPointer to SRAM
;*                param3 - x
;*
;* OUTPUT       : ZH     - HighPointer to SRAM
;*                ZL     - LowPointer to SRAM
;*
;***********************************************************************
UART_ReadX:
        push    ZL
        push    ZH
	push	param1
	push	param3

UART_ReadXAgain:
        cpi     param3,0                ;stop after x loops
        breq    UART_ReadXStop
        rcall   UART_Read
        st      Z+,param1               ;store char from UART
        dec     param3
        rjmp    UART_ReadXAgain

UART_ReadXStop:
        ldi     param1,0                ;last charakter must be 0
        st      Z,param1

	pop	param3
	pop	param1
        pop     ZH
        pop     ZL
        ret

;***********************************************************************
;* FUNCTION     : UART_WriteX
;*
;* CREATED      : 27.04.2000 by Jens Funke
;*
;* LAST CHANGED : 27.04.2000 by Jens Funke
;*
;* DESCRIPTION  : UART_WriteX will write x charakters to UART.
;*
;* INPUT        : ZH  =       HighPointer to SRAM
;*                ZL  =       LowPointer to SRAM
;*                param3  =       x
;*
;* OUTPUT       : ZH  =       HighPointer to SRAM
;*                ZL  =       LowPointer to SRAM
;*
;***********************************************************************
UART_WriteX:
        push    ZL
        push    ZH
	push	param3

UART_WriteXAgain:
        cpi     param3,0                ;stop after x loops
        breq    UART_WriteXStop
	ld	param1,Z+
        rcall   UART_Write
        dec     param3
        rjmp    UART_WriteXAgain

UART_WriteXStop:
	pop	param3
        pop     ZH
        pop     ZL
        ret

;***********************************************************************
;* FUNCTION     : UART_WriteSRAM
;*
;* CREATED      : 04.01.2000 by Jens Funke
;*
;* LAST CHANGED : 04.01.2000 by Jens Funke
;*
;* DESCRIPTION  : Writes a Byte from SRAM to UART
;*
;* INPUT        : ZH  =       HighPointer to SRAM
;*              : ZL  =       LowPointer to SRAM
;*
;* OUTPUT       : ZH  =       HighPointer to SRAM
;*              : ZL  =       LowPointer to SRAM
;*
;***********************************************************************
UART_WriteSRAM:
        push    ZH                  ;HighPointer
        push    ZL                  ;LowPointer
	push	param1

UART_WriteSRAMAgain:
        ld      param1,Z+               ;load char from SRAM
        cpi     param1,0                ;stop at null-character
        breq    UART_WriteSRAMStop
        rcall   UART_Write
        rjmp    UART_WriteSRAMAgain

UART_WriteSRAMStop:

	pop	param1
        pop     ZL
        pop     ZH
        ret

;***********************************************************************
;* FUNCTION	: UART_Print_As_Hex
;*
;* CREATED	: 14.05.2000 by Jens Funke
;*
;* LAST CHANGED : 14.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Print X data from position Z to UART
;*
;* INPUT        : ZH = HI(Pointer) to Startaddress (MSB)
;*                ZL = LO(Pointer) to Startaddress
;*                XH = HI(Pointer) to DataLenght (MSB)
;*                XL = LO(Pointer) to DataLenght
;*
;* OUTPUT       : -
;*
;***********************************************************************
UART_Print_As_Hex:
	push	param1
	push	temp1
	push	temp2
	push	XL
	push	XH
	push	ZL
	push	ZH
	ldi	temp2,0x01
	mov	temp1,ZL	; calculate the endaddress of the datafield
	add	XL,temp1	; it's neccessary for CPRW
	mov	temp1,ZH
	adc	XH,temp1
UART_Print_As_Hex_Loop:
	ld	param1,Z+
        rcall   UART_WriteHex
	CPRW	XL,XH,ZL,ZH,UART_Print_As_Hex_Loop
	pop	ZH
	pop	ZL
	pop	XH
	pop	XL
	pop	temp2
	pop	temp1
	pop	param1
	ret

;***********************************************************************
;* FUNCTION	: UART_Print_As_Bin
;*
;* CREATED	: 23.04.2000 by Jens Funke
;*
;* LAST CHANGED : 06.05.2000 by Jens Funke
;*
;* DESCRIPTION  : Convert Param3 from Hex to Bin and write it to UART
;*
;* INPUT        : ZH = HI(Pointer) to Startaddress (MSB)
;*                ZL = LO(Pointer) to Startaddress
;*                Param3 = Hex value which will converted to Bin.
;*
;* OUTPUT       : -
;*
;***********************************************************************
UART_Print_As_Bin:
	push	param3
        rcall   CONV_Hex2Bin
        ldi     param3,0x08             ;convert 8 Byte
        rcall   CONV_Bin2Ascii
        rcall   UART_WriteSRAM          ;write from SRAM
	pop	param3
	ret

  endif
	
;***********************************************************************
	endif

;*
;* $Log: uartext.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
