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
//* $Id: uart.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : UART-Routinen
//*  Version     : 1.5
//*
//*  Filename    : uart.c
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

#include "uart.h"


//***********************************************************************
//*
//* FUNCTION		: UART_Init
//*
//* CREATED		: 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 11.09.2000 by Nils Steenbock
//*
//* DESCRIPTION  : Initializes the builtin UART of the Atmel AT90S8515
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void UART_Init (void)
{
	outp ((u08)12, UBRR);
	sbi  (UCR, RXEN);
	sbi  (UCR, TXEN);
    cbi  (UCR, CHR9);
    sei();
	return;
}



//***********************************************************************
//*
//* FUNCTION		: UART_Write
//*
//* CREATED		: 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 11.09.2000 by Nils Steenbock
//*
//* DESCRIPTION  : Write single character to the UART
//*
//* INPUT        : u08 param1 - character to be written
//*
//* OUTPUT       : -
//*
//***********************************************************************

void UART_Write (const u08 param1)
{
	loop_until_bit_is_set(USR,UDRE);
	outp  (param1, UDR);
	//	LCD_WriteHex(param1);
}



//***********************************************************************
//*
//* FUNCTION		: UART_Read
//*
//* CREATED		: 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 11.09.2000 by Nils Steenbock
//*
//* DESCRIPTION  : Waits for received complete flag and then reads
//*				  single character from UART
//*
//* INPUT        : -
//*
//* OUTPUT       : u08 - character read
//*
//***********************************************************************


u08 UART_Read ()
{
	u08 param1;
	loop_until_bit_is_set(USR, RXC);
	param1=inp (UDR);
	return param1;
}


//***********************************************************************
//*
//* $Log: uart.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
