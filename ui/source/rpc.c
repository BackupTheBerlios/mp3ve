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
//* $Id: rpc.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title       : RPC-Routinen
//*  Version     : 1.5
//*
//*  Filename    : rpc.c
//*  Created     : 11.09.2000 by Nils Steenbock
//*  Last changed: 04.02.2001 by Nils Steenbock
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : RPC command sending for IDE-Board control
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : rpc.h
//*
//***********************************************************************

#include "rpc.h"

//***********************************************************************
//*
//* FUNCTION	 : RPC_SendCommand
//*
//* CREATED	 : 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 04.02.2001 by Nils Steenbock
//*
//* DESCRIPTION  : Send command to IDE-Interface
//*
//* INPUT        :
//*
//* OUTPUT       : -
//*
//***********************************************************************

void RPC_SendCommand (void)
{
	u08 cs, count, anz;

	//LCD_Clear();

	UART_Write('C');
	UART_Write('M');
	UART_Write('D');

	cs=0;

	UART_Write(RPC_Command[0]);

	cs+=RPC_Command[0];

	anz=RPC_Command[1];
	cs+=anz;
	UART_Write(anz);

	for (count=0;count<anz;count++)
	  {
	    cs+=RPC_Command[count+2];
	    UART_Write(RPC_Command[count+2]);
	  }

	UART_Write(~cs+1);
}

//***********************************************************************
//*
//* $Log: rpc.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
