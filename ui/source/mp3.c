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
//* $Id: mp3.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title 	 : MP3-Stubs
//*  Version     : 1.5
//*
//*  Filename    : mp3.c
//*  Created     : 11.09.2000 by Nils Steenbock
//*  Last changed: 01.03.2001 by Nils Steenbock
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : Stubs for MP3 Control
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : mp3.h
//*
//***********************************************************************

#include "mp3.h"

//***********************************************************************
//*
//* FUNCTION	 : MP3_Play
//*
//* CREATED	 : 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.01.2001 by Thomas Gutzeit
//*
//* DESCRIPTION  : Starts play mode of IDE-Board
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void MP3_Play(void)
{
  RPC_Command[0]=0x02;
  RPC_Command[1]=0x01;
  RPC_SendCommand();
  LCD_Wait(50);
}



//***********************************************************************
//*
//* FUNCTION	 : MP3_Stop
//*
//* CREATED		 : 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.01.2001 by Thomas Gutzeit
//*
//* DESCRIPTION  : Stops playing MP3-Music
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void MP3_Stop(void)
{
  RPC_Command[0]=0x01;
  RPC_Command[1]=0x01;
  RPC_SendCommand();
  LCD_Wait(50);

}


//***********************************************************************
//*
//* FUNCTION	 : MP3_Pause
//*
//* CREATED	 : 24.10.2000 by Nils Steenbock
//*
//* LAST CHANGED : 22.02.2001 by Thomas Gutzeit
//*
//* DESCRIPTION  : Stops playing MP3-Music
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void MP3_Pause(void)
{
  //RPC_SendCommand(RPC_MP3Pause);

  RPC_Command[0]=0x09;
  RPC_Command[1]=0x01;
  RPC_SendCommand();
  LCD_Wait(50);
}


//***********************************************************************
//*
//* FUNCTION	 : MP3_Next
//*
//* CREATED	 : 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 01.03.2001 by Nils Steenbock
//*
//* DESCRIPTION  : Sets next song and starts playing immediately, if in
//*                play mode
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void MP3_Next(u08 song)
{
  u08 tmp1, tmp2;

  RPC_Command[0]=0x0c;
  RPC_Command[1]=0x08;
  RPC_Command[2]=TOC_getStartsector(song,3);
  RPC_Command[3]=TOC_getStartsector(song,2);
  RPC_Command[4]=TOC_getStartsector(song,1);
  RPC_Command[5]=TOC_getStartsector(song,0);


  RPC_Command[6]=TOC_getLength(song,3);
  RPC_Command[7]=TOC_getLength(song,2);
  RPC_Command[8]=TOC_getLength(song,1);
  RPC_Command[9]=TOC_getLength(song,0);

  tmp1=0;
  tmp1=(TOC_getLength(song,1)>>3)&31;
  tmp2=(TOC_getLength(song,2)<<5)&224;
  tmp1|=tmp2;
  RPC_Command[9]=tmp1;

  tmp1=(TOC_getLength(song,2)>>3)&31;
  tmp2=(TOC_getLength(song,3)<<5)&224;
  tmp1|=tmp2;
  RPC_Command[8]=tmp1;

  tmp1=(TOC_getLength(song,3)>>3)&31;
  RPC_Command[7]=tmp1;

  RPC_Command[6]=0;


  RPC_SendCommand();
  LCD_Wait(50);

  RPC_Command[0]=0x0b;
  RPC_Command[1]=0x01;

  RPC_SendCommand();
  LCD_Wait(50);

}

//***********************************************************************
//*
//* $Log: mp3.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
