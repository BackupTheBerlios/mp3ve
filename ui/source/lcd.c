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
//* $Id: lcd.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : LCD-Routinen
//*  Version     : 1.5
//*
//*  Filename    : lcd.c
//*  Created     : 05.07.2000 by Thomas Gutzeit
//*  Last changed: 21.12.2000 by Marc Dressler
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : to control the display
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : lcd.h
//* 			   spi.h
//*
//***********************************************************************


#include "lcd.h"



/***********************************************************************
;* FUNCTION	:		void LCD_BitConvert_h(u08 data_byte)
;*
;* CREATED	: 		05.07.2000
;*
;* LAST CHANGED : 	10.07.2000
;*
;* DESCRIPTION  : 	converts the high-nibble of the LCD-Databit for the SPI
;*
;* INPUT        : 	lcd_databyte
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/


void LCD_BitConvert_h(u08 data_byte)
{
	lcd_state = lcd_state & (0xff-((1<<lcd_d7)+(1<<lcd_d6)+(1<<lcd_d5)+(1<<lcd_d4)));
	if ((data_byte & 0x80)!=0) lcd_state=lcd_state | (1<<lcd_d7);	//set bit4
	if ((data_byte & 0x40)!=0) lcd_state=lcd_state | (1<<lcd_d6);	//set bit0
	if ((data_byte & 0x20)!=0) lcd_state=lcd_state | (1<<lcd_d5);	//set bit5
	if ((data_byte & 0x10)!=0) lcd_state=lcd_state | (1<<lcd_d4);	//set bit1
}



/***********************************************************************
;* FUNCTION	:		void LCD_BitConvert_l(u08 data_byte)
;*
;* CREATED	: 		05.07.2000
;*
;* LAST CHANGED : 	10.07.2000
;*
;* DESCRIPTION  : 	converts the low-nibble of the LCD-Databit for the SPI
;*
;* INPUT        : 	lcd_databyte
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/
void LCD_BitConvert_l(u08 data_byte)
{
	lcd_state = lcd_state & (0xff-((1<<lcd_d3)+(1<<lcd_d2)+(1<<lcd_d1)+(1<<lcd_d0)));
	if ((data_byte & 0x08)!=0) lcd_state=lcd_state | (1<<lcd_d3);	//set bit
	if ((data_byte & 0x04)!=0) lcd_state=lcd_state | (1<<lcd_d2);	//set bit
	if ((data_byte & 0x02)!=0) lcd_state=lcd_state | (1<<lcd_d1);	//set bit
	if ((data_byte & 0x01)!=0) lcd_state=lcd_state | (1<<lcd_d0);	//set bit
}




/***********************************************************************
;* FUNCTION	:		LCD_Init
;*
;* CREATED	: 		27.05.2000 by Thomas Gutzeit
;*
;* LAST CHANGED : 	10.07.2000 by Thomas Gutzeit
;*
;* DESCRIPTION  : 	initialize the LC-display
;*
;* INPUT        : 	-
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/

void LCD_Init (void)
{
	LCD_Wait(30);
	lcd_state = 0;				// led off, rs,rw,e=0
	LCD_Enable();
	LCD_BitConvert_h(lcd_protocol);
	LCD_Enable();

	LCD_WriteCom (lcd_set);
	LCD_WriteCom (lcd_on);
	LCD_WriteCom (lcd_clear);
	LCD_WriteCom (lcd_entry);
}



/***********************************************************************
;* FUNCTION	:		void LCD_Enable()
;*
;* CREATED	:		27.05.2000
;*
;* LAST CHANGED :	10.07.2000
;*
;* DESCRIPTION  :	sets the enable-bit, the data is take from bus
;*					with the falling edge of the enable-signal
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/


void LCD_Enable ()
{
	key_state = SPI_Cycle (~led_state, lcd_state);
	lcd_state = lcd_state | (1<<lcd_e);
	key_state = SPI_Cycle (~led_state, lcd_state);
	lcd_state = lcd_state & (0xff-(1<<lcd_e));
	key_state = SPI_Cycle (~led_state, lcd_state);
}


/***********************************************************************
;* FUNCTION	:		void LCD_Wait(unsigned int t)
;*
;* CREATED	:		27.05.2000
;*
;* LAST CHANGED :	10.07.2000
;*
;* DESCRIPTION  :	wait until display is ready
;*
;* INPUT        : 	unsigned int t
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/

void LCD_Wait (unsigned int t)
{
	u08 i;
	u08 j;
	u08 k;
	for(i=1;i<t;i++)
	for(j=1;j<t;j++)
	for(k=1;k<t;k++){
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
		asm("nop");
	}
}



/***********************************************************************
;* FUNCTION	:		void LCD_WriteCom (u08 data)
;*
;* CREATED	:		27.05.2000
;*
;* LAST CHANGD :	10.07.2000
;*
;* DESCRIPTION :	send data to display
;*
;* INPUT       : 	u08 data
;*
;* OUTPUT      : 	-
;*
;***********************************************************************/
void LCD_WriteCom (u08 data)
{	cli();
	lcd_state = lcd_state & (0xff-(1<<lcd_rs));
	LCD_Wait(12);	//20
	LCD_BitConvert_h (data);
	LCD_Enable();
	LCD_Wait(12);	//20
	LCD_BitConvert_l (data);
	LCD_Enable();
	sei();
}


/***********************************************************************
;* FUNCTION	:		void LCD_Write(u08 data)
;*
;* CREATED	:		27.05.2000
;*
;* LAST CHANGED :	10.07.2000
;*
;* DESCRIPTION  :	send data-byte to LC-Display
;*
;* INPUT        : 	u08 data
;*
;* OUTPUT       : 	-
;*
;***********************************************************************/

void LCD_Write (u08 data)
{
		cli();
		LCD_Wait(12);	//12
		LCD_BitConvert_h (data);
		lcd_state = lcd_state | (1<<lcd_rs);	//0x80;	//10000000b; (set RS)
		LCD_Enable();
		LCD_BitConvert_l (data);
		lcd_state = lcd_state | (1<<lcd_rs);	//0x80;	//10000000b; (set RS)
		LCD_Enable();
	    sei();
}



/***********************************************************************
;* FUNCTION	:		void LCD_WriteTxt(char s[])
;*
;* CREATED	:		26.06.2000
;*
;* LAST CHANGED :	10.07.2000
;*
;* DESCRIPTION  :	write text to LC-display
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/


void LCD_WriteTxt(const char* s)
{
	const int i=strlen(s);
	int j;
	j=0;
	while (j<i)
	{
		LCD_Write(s[j]);
		j++;
	}

}



/***********************************************************************
;* FUNCTION	:		void LCD_SetCursor (u08 position)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	Set cursor to free position
;*
;* INPUT        : 	position
;*
;* OUTPUT       :	-
;*
;***********************************************************************/

void LCD_SetCursor (u08 position)
{
	position=(position | (1<<7));		//set lcd bit7 to 1
	LCD_WriteCom (position);
}


/***********************************************************************
;* FUNCTION	:		void LCD_On (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	turn on the display
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/

void LCD_On (void)
{
	LCD_WriteCom (lcd_on);
}



/***********************************************************************
;* FUNCTION	:		void LCD_Off (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	turn off the display
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/

void LCD_Off (void)
{
	LCD_WriteCom (lcd_off);
}


/***********************************************************************
;* FUNCTION	:		void LCD_Clear (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	clear the display
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/
void LCD_Clear (void)
{
	LCD_WriteCom (lcd_clear);
}



/***********************************************************************
;* FUNCTION	:		void LCD_Home (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	set the cursor at home-position
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/
void LCD_Home (void)
{
	LCD_WriteCom (lcd_home);
}


/***********************************************************************
;* FUNCTION	:		void LCD_CursorLeft (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	move the cursor one left
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/
void LCD_CursorLeft (void)
{
	LCD_WriteCom(lcd_cursorleft);
}


/***********************************************************************
;* FUNCTION	:		void LCD_CursorRight (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	move the cursor one right
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/
void LCD_CursorRight (void)
{
	LCD_WriteCom(lcd_cursorright);
}



/***********************************************************************
;* FUNCTION	:		void LCD_DisplayLeft (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	move the display one left
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/
void LCD_DisplayLeft (void)
{
	LCD_WriteCom(lcd_displayleft);
}


/***********************************************************************
;* FUNCTION	:		void LCD_DisplayRight (void)
;*
;* CREATED	:		30.07.2000
;*
;* LAST CHANGED :	30.07.2000
;*
;* DESCRIPTION  :	move the display one right
;*
;* INPUT        : 	-
;*
;* OUTPUT       :	-
;*
;***********************************************************************/
void LCD_DisplayRight (void)
{
	LCD_WriteCom(lcd_displayright);
}


void LCD_WriteHex(u08 bit)
{
  u08 t1;

  t1=(u08)bit/16;
  t1+=0x30;
  if (t1>0x39) t1+=0x13;
  LCD_Write(t1);
  t1=(u08)(bit-16*t1+0x30);
  if (t1>0x39) t1+=0x13;
  LCD_Write(t1);
}

//***********************************************************************
//*
//* $Log: lcd.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
