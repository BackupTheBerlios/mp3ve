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
//* $Id: dev.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title 	 : DEV-Stubs
//*  Version     : 1.5
//*
//*  Filename    : dev.c
//*  Created     : 15.12.2000 by Nils Steenbock
//*  Last changed: 04.02.2001 by Nils Steenbock
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : Stubs for Device Control
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : dev.h
//*
//***********************************************************************

#include "dev.h"
#include "rpc.h"

//***********************************************************************
//*
//* FUNCTION	 : DEV_Select_Master
//*
//* CREATED	 : 15.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 04.02.2001 by Nils Steenbock
//*
//* DESCRIPTION  : Selects Master IDE-Device
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void DEV_Select_Master(void)
{
  RPC_Command[0]=0x05;
  RPC_Command[1]=0x01;

  RPC_SendCommand();

  LCD_Wait(50);
}


//***********************************************************************
//*
//* FUNCTION	 : DEV_Select_Slave
//*
//* CREATED	 : 15.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 04.02.2001 by Nils Steenbock
//*
//* DESCRIPTION  : Selects Master IDE-Device
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void DEV_Select_Slave(void)
{
  RPC_Command[0]=0x06;
  RPC_Command[1]=0x01;

  RPC_SendCommand();

  LCD_Wait(50);

}


//***********************************************************************
//*
//* FUNCTION	 : DEV_Get_Info
//*
//* CREATED	 : 04.02.2001 by Nils Steenbock
//*
//* LAST CHANGED : 04.02.2001 by Nils Steenbock
//*
//* DESCRIPTION  : Selects Master IDE-Device
//*
//* INPUT        : -
//*
//* OUTPUT       : Device Info - Byte
//*
//***********************************************************************

u08 DEV_Get_Info(void)
{
  u08 answer;
  cli();
  RPC_Command[0]=0x1e;
  RPC_Command[1]=0x01;

  RPC_SendCommand();

LCD_Wait(50);
  answer=UART_Read();
  sei();
  return (answer);

}



//***********************************************************************
//*
//* $Log: dev.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
