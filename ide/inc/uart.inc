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
;* $Id: uart.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : UART Include-File
;*  Version         : 1.5
;*
;*  Filename        : uart.inc
;*  Created         : 13.11.1999 by Marc Dressler
;*  Last changed    : 20.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : all
;*  Description     : basic uart functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************

	ifndef	uartinc
uartinc		=	1

		if	mompass=1
                 message "UART Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************
	
	INCLUDE	"uart.mac"

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

UART_BAUD       EQU       38400		; Baud rate
;UART_BAUD       EQU       9600		; Baud rate
UART_Hz         EQU       8*1000*1000	; Processor speed (Hz)

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

UART_UBRR       EQU       (UART_Hz/(16*UART_BAUD))-1

;***********************************************************************
;* ASCII codes
;***********************************************************************

CR              EQU       10      ;
LF              EQU       13      ;
BS              EQU        8      ;
ESC             EQU       27      ;
CLS             EQU       12      ;

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: UART_Init
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : Initialize UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_Init:
        push    temp1
        ldi     temp1, UART_UBRR
        out     UBRR, temp1              ;set baud-rate
        sbi     UCR, RXEN                ;enable receiver
        sbi     UCR, TXEN                ;enable transmitter
        cbi     UCR, CHR9                ;set 8-bit character
        pop     temp1
        ret

;***********************************************************************
;* FUNCTION	: UART_WaitRead
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : wait for character
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WaitRead:
        sbis    USR, RXC		;wait until Receiver Complete
        rjmp    UART_WaitRead
        ret

;***********************************************************************
;* FUNCTION	: UART_WaitWrite
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : wait for byte transmitted
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WaitWrite:
	sbis    USR, UDRE		;wait until Data Register Empty
;	sbis    USR, TXC		;wait until Transmit Complete
	rjmp    UART_WaitWrite
	ret

;***********************************************************************
;* FUNCTION	: UART_Write
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : write charater in register param1 to UART
;*
;* INPUT        : param1 - character
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_Write:
        rcall   UART_WaitWrite		;wait until ready to send
	
        out     UDR, param1		;write character to UART
        ret

;***********************************************************************
;* FUNCTION	: UART_WriteHex
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : write value in register param1 as hex-code to UART
;*
;* INPUT        : param1 - value
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WriteHex:
        push    temp1
        push    temp2

        mov     temp1, param1
        andi    temp1, 0b11110000	;high nibble
        swap    temp1
        rcall   UART_WHHB

        mov     temp1, param1
        andi    temp1, 0b00001111	;low nibble
        rcall   UART_WHHB

        pop     temp2
        pop     temp1
        ret

UART_WHHB:
        cpi     temp1, 10		;test if nibble exceeds 9
        brge    UART_WHGN
        ldi     temp2, 48		;nibble is a number
        rjmp    UART_WHO

UART_WHGN:
        ldi     temp2, 55		;nibble is A..F
UART_WHO:
        add     temp1, temp2		;make nibble a character
        push    param1			;store param1

        mov     param1, temp1		;set character as new argument
        rcall   UART_WRITE		;write character
        pop     param1			;restore old argument
        ret

;***********************************************************************
;* FUNCTION	: UART_Read
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : read character from UART to register param1
;*
;* INPUT        : -
;*
;* OUTPUT       : param1 - character
;*
;***********************************************************************

UART_Read:
        rcall   UART_WaitRead		;wait until character read
        in      param1, UDR		;get character
        ret

;***********************************************************************
;* FUNCTION	: UART_WriteTxt
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : Write characters from flash start address (Z-pointer)
;* 		  up to first null-character
;* 		  Z-pointer - start address x 2 (WORD oriented !)
;*
;* INPUT        : zl - low start adress
;*                zh - high start adress
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WriteTxt:
        push    r0
        push    param1
UART_WTL:
        lpm                             ;load byte to R0
        mov     param1, r0
        cpi     param1, 0x00		;compare with 0x00
        breq    UART_WTret		;jump, if text-end reached

        rcall   UART_Write		;write character to UART

        adiw    zl, 0x01		;increment flash-pointer
        rjmp    UART_WTL		;go to next character
UART_WTret:
        pop     param1
        pop     r0
        ret

;***********************************************************************
;* FUNCTION	: UART_WriteCRLF
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : write CR and LF to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WriteCRLF:
        push    param1
        ldi     param1, 10		;CR
        rcall   UART_Write
        ldi     param1, 13		;LF
        rcall   UART_Write
        pop     param1
        ret

;***********************************************************************
;* FUNCTION	: UART_WriteBS
;*
;* CREATED	: 13.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 14.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : write BS to UART
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

UART_WriteBS:
        push    param1
        ldi     param1, 8		;BS
        rcall   UART_Write
        pop     param1
        ret

;***********************************************************************

	endif

;*
;* $Log: uart.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
