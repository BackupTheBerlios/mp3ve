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
//* $Id: sys.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title 	 : SYS-Stubs
//*  Version     : 1.5
//*
//*  Filename    : sys.c
//*  Created     : 15.12.2000 by Nils Steenbock
//*  Last changed: 04.02.2001 by Nils Steenbock
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : Stubs for basic System functions
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : sys.h
//*                rpc.h
//*
//***********************************************************************

#include "sys.h"
#include "rpc.h"

//***********************************************************************
//*
//* FUNCTION	 : SYS_Ping
//*
//* CREATED	 : 04.02.2000 by Nils Steenbock
//*
//* LAST CHANGED : 04.02.2001 by Nils Steenbock
//*
//* DESCRIPTION  : Submit Ping to IDE-Board
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void SYS_Ping(void)
{
  // Thomas : clear alive flag

  RPC_Command[0]=0x10;
  RPC_Command[1]=0x01;
  RPC_Command[2]='P';

  RPC_SendCommand();

  LCD_Wait(50);
}

//***********************************************************************
//*
//* $Log: sys.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*







