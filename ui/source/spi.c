/*
    MP3VE - Standalone MP3 Player
    Copyright (C) 2000,2001,2002  Marc Dressler, Jens Funke,
                                  Thomas Gutzeit, Nils Steenbock

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    info@mp3ve.de
*/

//***********************************************************************
//* $Id: spi.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : SPI-Routines
//*  Version     : 1.5
//*
//*  Filename    : spi.c
//*  Created     : 10.05.2000 by Thomas Gutzeit
//*  Last changed: 21.12.2000 by Marc Dressler
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : to control the spi-interface
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : spi.h
//*
//***********************************************************************




#include "spi.h"



/***********************************************************************
;* FUNCTION	:		SPI_Init
;*
;* CREATED	: 		10.05.2000 by Thomas Gutzeit
;*
;* LAST CHANGED :	10.07.2000
;*
;* DESCRIPTION  : 	initialize spi hardware
;*
;* INPUT        : 	-
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/


void SPI_Init (void)
{	u08 temp;

	sbi (DDRB,PB4);	/*SS = output*/
	cbi (PORTB,PB4);	/*pull-up on*/

	sbi (DDRB,PB5);	/*MOSI = output */
	sbi (PORTB,PB5);	/*pull-up on*/

	cbi (DDRB,PB6);	/*MISO = input */
	cbi (PORTB,PB6);	/*pull-up on*/


	sbi (DDRB,PB7);	/*SCK = output*/
	cbi (PORTB,PB7);	/*pull-up on*/

	outp (0x1C,SPCR);
	/*outp (0b0001.1100,SPCR);	init SPI (bit7: SPIE; SPI Interrput Enable,
										bit6: SPE; SPI Enable
										bit5: DORD; Data Order: 1=>LSB is transmitted first
																2=>MSB is transmitted first
										bit4: MSTR; Master/Slave Selecct: 1=> Master SPI mode
										bit3: CPOL; Clock Polarity: 1=> SCK is high when idle
																	2=> SCK is low when idle
										bit2: CPHA; Clock Phase
										bits1/0: SPR1,SPR0; SPI Clock Rate Select 1 and 0  )
																								*/
	temp = inp(SPSR);
	sbi(SPCR,SPE);
	outp(temp,SPDR);
}




/***********************************************************************
;* FUNCTION	:		SPI_Wait
;*
;* CREATED	: 		10.05.2000 by Thomas Gutzeit
;*
;* LAST CHANGED : 	10.07.2000 by Thomas Gutzeit
;*
;* DESCRIPTION  : 	wait until ready to send / read
;*
;* INPUT        : 	-
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/
void SPI_Wait (void)
{
loop_until_bit_is_set(SPSR, SPIF);	/*wait until transfer complete*/
										/*see: iomacros.h */
}



/***********************************************************************
;* FUNCTION	:		SPI_Write
;*
;* CREATED	: 		10.05.2000 by Thomas Gutzeit
;*
;* LAST CHANGED : 	10.07.2000 by Thomas Gutzeit
;*
;* DESCRIPTION  : 	write character to spi-port
;*
;* INPUT        : 	param of type u08
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/


u08 SPI_Write (u08 param1)
{
u08	temp;

	SPI_Wait ();

	temp = inp(SPDR);

	sbi(SPCR,MSTR);
	sbi(SPCR,SPE);
	outp (param1,SPDR );

	return temp;
}


/***********************************************************************
;* FUNCTION	:		SPI_Clk
;*
;* CREATED	: 		10.05.2000 by Thomas Gutzeit
;*
;* LAST CHANGED : 	10.07.2000 by Thomas Gutzeit
;*
;* DESCRIPTION  : 	Clk-Signal to /SS
;*
;* INPUT        : 	-
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/

void SPI_Clk (void)
{
	sbi(PORTB,PB4);
	cbi(PORTB,PB4);
}



/***********************************************************************
;* FUNCTION	:		SPI_Cycle
;*
;* CREATED	: 		22.05.2000 by Thomas Gutzeit
;*
;* LAST CHANGED : 	10.07.2000 by Thomas Gutzeit
;*
;* DESCRIPTION  : 	Clk-Signal to /SS
;*
;* INPUT        : 	u08 led_param, u08 lcd_param
;*
;* OUTPUT       : 	u08 key_param
;*
;***********************************************************************/

u08 SPI_Cycle (u08 led_param, u08 lcd_param)
{
	u08 key_param;

	//led_param=0xff;

	SPI_Clk();
	SPI_Write(led_param);	//LED
	key_param = SPI_Write(lcd_param);	//LCD
	SPI_Wait();
	SPI_Clk();
	return key_param;
}

//***********************************************************************
//*
//* $Log: spi.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*








