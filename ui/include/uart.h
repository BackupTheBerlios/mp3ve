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
//* $Id: uart.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : UART-Routinen
//*  Version     : 1.5
//*
//*  Filename    : uart.h
//*  Created     : 11.09.2000 by Nils Steenbock
//*  Last changed: 21.12.2000 by Marc Dressler
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : Basic functions for UART needed by RPC
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : uart.h
//*
//***********************************************************************

#include <io.h>
#include "bitfunction.h"
#include "lcd.h"
#include <interrupt.h>
#include <sig-avr.h>

#ifndef __UART_H__
#define __UART_H__

#define UART_BAUD 38400
#define UART_Hz 8*1000*1000
#define UART_UBRR (UART_Hz/(UART_BAUD*161)-1)




void UART_Init (void);

void UART_Write	(const u08 param1);
u08 UART_Read (void);

#endif

//***********************************************************************
//*
//* $Log: uart.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
