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
;* $Id: spi.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : SPI Include-File
;*  Version         : 1.5
;*
;*  Filename        : spi.inc
;*  Created         : 27.11.1999 by Marc Dressler
;*  Last changed    : 20.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : access routines for SPI interface
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : i2c.inc
;*
;***********************************************************************

	ifndef	spiinc	; prevent multiple binding

spiinc	equ	1

	if	mompass=1
		message "SPI Include-File V1.5 (C)2000 Marc Dressler"
	endif

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: SPI_Init
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 05.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : initialize spi hardware
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

SPI_Init:
	push	temp1

	sbi	DDRB, PB4			;/SS = output
	sbi	PORTB, PB4			;pull-up on

	sbi	DDRB, PB5			;MOSI = output
	sbi	PORTB, PB5			;pull-up on

	sbi	DDRB, PB7			;SCK = output
	sbi	PORTB, PB7			;pull-up on

  ifndef signals_inverted
	ldi	temp1, (1+SPIE)+(1<<CPOL)+(1<<CPHA)+(1<<SPR0)	; f/16=500kbit (ok)
  endif
  
  ifdef signals_inverted
	ldi	temp1, (1+SPIE)+(1<<CPHA)+(1<<SPR0)	; f/16=500kbit (ok)
  endif
  
  
;	ldi	temp1, (1+SPIE)+(1<<CPOL)+(1<<SPR0)	; f/16=500kbit (error/error)
;	ldi	temp1, (1+SPIE)+(1<<CPHA)+(1<<SPR0)	; f/16=500kbit (ok/error-irq)
;	ldi	temp1, (1+SPIE)+(1<<SPR0)		; f/16=500kbit (???)

;	ldi	temp1, (1+SPIE)+(1<<CPOL)+(1<<SPR1)	; f/64=125kbit (???)
;	ldi	temp1, (1+SPIE)+(1<<CPOL)+(1<<CPHA)+(1<<SPR1)	; f/64=125kbit (ok (112kbps Song) without errors)

;	ldi	temp1, (1+SPIE)+(1<<CPOL)+(1<<CPHA)	; f/4=2000kbit (ok/?) 


	out	SPCR, temp1			;init SPI
	in	temp1, SPSR			;clear flags

	sbi	SPCR, MSTR
	sbi	SPCR, SPE			;enable SPI
	out	SPDR, temp1			;send dummy to activate SPI

	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: SPI_Stop
;*
;* CREATED	: 18.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 05.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : set flags to stop spi
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

SPI_Stop:
	cbi     SPCR, SPE                 ; enable off
        cbi     SPCR, MSTR                ; master off
	ret

;***********************************************************************
;* FUNCTION	: SPI_WaitWrite
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 05.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : wait until ready to send
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

SPI_WaitWrite:
	sbis	SPSR, SPIF			;wait until transfer complete
	rjmp	SPI_WaitWrite
	ret

;***********************************************************************
;* FUNCTION	: SPI_Write
;*
;* CREATED	: 27.11.1999 by Marc Dressler
;*
;* LAST CHANGED : 20.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : write character to spi-port
;*
;* INPUT        : param1 - character
;*
;* OUTPUT       : -
;*
;***********************************************************************

SPI_Write:
;	sbis	SPSR, SPIF			;wait until transfer complete
;	rjmp	SPI_Write

	sbi	SPCR, MSTR
	sbi	SPCR, SPE			;enable SPI
	
	out	SPDR, param1
	ret

;***********************************************************************

	endif

;*
;* $Log: spi.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
