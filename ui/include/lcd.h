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
//* $Id: lcd.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : LCD-Header
//*  Version     : 1.5
//*
//*  Filename    : lcd.h
//*  Created     : 05.07.2000 by Thomas Gutzeit
//*  Last changed: 21.12.2000 by Marc Dressler
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : header-file for lcd.c
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : spi.h
//*                interrupt.h
//*
//*
//***********************************************************************
#include "spi.h"
#include "interrupt.h"

#ifndef _lcd_h__
#define _lcd_h__



#define lcd_d0 1
#define lcd_d1 5
#define lcd_d2 0
#define lcd_d3 4
#define lcd_d4 1
#define lcd_d5 5
#define lcd_d6 0
#define lcd_d7 4
#define lcd_e  6
#define lcd_rw 2
#define lcd_rs 7
#define lcd_led 3

#define lcd_FSM_line_no1 0x00
#define lcd_line_no2 0x29

#define lcd_protocol 0x20	//00100000b		"4bit protocol"
#define lcd_reset 	0x02	//00000010b		"Cursor at home"
#define lcd_set 	0x28	//00101000b		"Function set"
#define lcd_clear	0x01	//00000001b		"Clear display"
#define lcd_home	0x02	//00000010b		"Cursor at home"
#define lcd_entry	0x06	//00000110b		"Enty mode set"
#define lcd_on		0x0C	//00001100b
#define lcd_off		0x0B	//00001011b
#define lcd_displayleft		0x18	//00011000b
#define lcd_displayright 	0x1C	//00011100b
#define lcd_cursorleft		0x10	//00010000b
#define lcd_cursorright		0x14	//00010100b


u08 lcd_state;
u08 led_state;
u08 key_state;
u08 key_state_new;

void LCD_Init (void);
void LCD_BitConvert_h(u08 bit);
void LCD_BitConvert_l(u08 bit);
void LCD_Wait (unsigned int t);
void LCD_Enable (void);
void LCD_WriteCom (u08 data);
void LCD_Write (u08 data);
void LCD_WriteTxt (const char* s);
void LCD_On (void);
void LCD_Off (void);
void LCD_Clear (void);
void LCD_Home (void);
void LCD_SetCursor (u08 position);
void LCD_CursorLeft (void);
void LCD_CursorRight (void);
void LCD_DisplayLeft (void);
void LCD_DisplayRight (void);
void LCD_WriteHex(u08 bit);


#endif

//***********************************************************************
//*
//* $Log: lcd.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
