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
//* $Id: toc.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//**********************************************************************
//*
//*  Title       : reading the TOC
//*  Version     : 1.5
//*
//*  Filename    : toc.c
//*  Created     : 11.09.2000 by Nils Steenbock
//*  Last changed: 04.02.2001 by Nils Steenbock
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : the Toc-Read routine
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : toc.h
//*
//***********************************************************************

#include "toc.h"

TOC_Entry toc1[100];
TOC_Entry toc2[100];
TOC_Entry toc3[100];


//***********************************************************************
//*
//* FUNCTION	 : TOC_Read
//*
//* CREATED	 : 11.09.2000 by Nils Steenbock
//*
//* LAST CHANGED : 04.02.2001 by Nils Steenbock
//*
//* DESCRIPTION  : Reads Joliet-TOC from CD
//*
//* INPUT        : -
//*
//* OUTPUT       : Number of directory entries in toc-Array
//*
//***********************************************************************


int TOC_Read(void)
{
  u08 buf;
  u08  fsm1,fsm2;
  int count;
  long b;
  cli();

  RPC_Command[0]=0x04;
  RPC_Command[1]=0x01;
  RPC_SendCommand();

  fsm1=0;
  fsm2=0;
  count=0;
  b=0;

  while(!(fsm2==3))
    {
      key_state_new=~(SPI_Cycle(~led_state, lcd_state));
      if (key_state_new==128) break;

      buf=UART_Read();
      {
	if (fsm1>2)
	  {
	    if (fsm1==73)
	      {
		fsm1=0;
		fsm2=0;

		count++;
		if (count==299) fsm2=3;
		TOC_setDir(count,0);
		TOC_setStartsector(count,0,0);
		TOC_setStartsector(count,1,0);
		TOC_setStartsector(count,2,0);
		TOC_setStartsector(count,3,0);

		TOC_setLength(count,0,0);
		TOC_setLength(count,1,0);
		TOC_setLength(count,2,0);
		TOC_setLength(count,3,0);

		TOC_setLevel(count,0);
	      }

	    if ((fsm1==12)&&(buf==2)) TOC_setDir(count,1);
	    if ((fsm1==12)&&(buf==0)) TOC_setDir(count,0);
	    if (fsm1==3) TOC_setLevel(count,buf);
	    if (fsm1==4) TOC_setStartsector(count,0,buf);
	    if (fsm1==5) TOC_setStartsector(count,1,buf);
	    if (fsm1==6) TOC_setStartsector(count,2,buf);
	    if (fsm1==7) TOC_setStartsector(count,3,buf);
	    if (fsm1==8) TOC_setLength(count,0,buf);
	    if (fsm1==9) TOC_setLength(count,1,buf);
	    if (fsm1==10) TOC_setLength(count,2,buf);
	    if (fsm1==11) TOC_setLength(count,3,buf);


	    if ((fsm1>12)&&(fsm1<73))
	      {

		TOC_setTitleChar(count,(fsm1-13),buf);
	      }
	    fsm1++;
	  }
	if ((fsm1==0)&&(buf=='T')) fsm1=1;
	if ((fsm1==1)&&(buf=='O')) fsm1=2;
	if ((fsm1==2)&&(buf=='C')) fsm1=3;


	if (fsm2==2)
	  {
	    if (buf=='D')
	      fsm2=3;
	    else fsm2=0;
	  }

	if (fsm2==1)
	  {
	    if (buf=='N')
	      fsm2=2;
	    else fsm2=0;
	  }
	if ((fsm2==0)&&(buf=='E')) fsm2=1;
      }

    }
  sei();
  LCD_Wait(50);
  return count ;
}



//***********************************************************************
//*
//* FUNCTION	 : TOC_isDir
//*
//* CREATED	 : 04.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 07.12.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : get information about the directory flag
//*
//* INPUT        : index - index of TOC_Entry
//*
//* OUTPUT       : true - if directory, false otherwise
//*
//***********************************************************************

u08 TOC_isDir(int index)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) return(toc1[index].dirflag);
	if (arr==1) return(toc2[index].dirflag);
		else return(toc3[index].dirflag);
}



//***********************************************************************
//*
//* FUNCTION	 : TOC_getLevel
//*
//* CREATED	 : 04.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 07.12.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : get level information on TOC_Entry
//*
//* INPUT        : index - index of TOC_Entry
//*
//* OUTPUT       : level
//*
//***********************************************************************



u08 TOC_getLevel(int index)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) return(toc1[index].level);
	if (arr==1) return(toc2[index].level);
	 else return(toc3[index].level);

}


//***********************************************************************
//*
//* FUNCTION	 : TOC_getTitleChar
//*
//* CREATED	 : 04.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 07.12.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : get single character from title string
//*
//* INPUT        : index - index of TOC_Entry
//*                offset - index of character in title string
//*
//* OUTPUT       : character
//*
//***********************************************************************


u08 TOC_getTitleChar(int index, u08 offset)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) return(toc1[index].title[offset]);
	if (arr==1) return(toc2[index].title[offset]);
		else return(toc3[index].title[offset]);

}


//***********************************************************************
//*
//* FUNCTION	 : TOC_getStartsector
//*
//* CREATED	 : 09.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.12.2000 by Nils Steenbock
//*
//* DESCRIPTION  : get single byte from startsector
//*
//* INPUT        : index - index of TOC_Entry
//*                offset - index of byte
//*
//* OUTPUT       : value of startsector index at offset
//*
//***********************************************************************


u08 TOC_getStartsector(int index, u08 offset)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;

	if (arr==0) return (toc1[index].startsector[offset]);
	if (arr==1) return (toc2[index].startsector[offset]);
	 else return (toc3[index].startsector[offset]);

}



//***********************************************************************
//*
//* FUNCTION	 : TOC_getStartsector
//*
//* CREATED	 : 09.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.12.2000 by Nils Steenbock
//*
//* DESCRIPTION  : get single byte from length
//*
//* INPUT        : index - index of TOC_Entry
//*                offset - index of byte
//*
//* OUTPUT       : value of length index at offset
//*
//***********************************************************************

u08 TOC_getLength(int index, u08 offset)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) return (toc1[index].length[offset]);
	if (arr==1) return (toc2[index].length[offset]);
	 else return (toc3[index].length[offset]);

}



//***********************************************************************
//*
//* FUNCTION	 : TOC_setDir
//*
//* CREATED	 : 09.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.12.2000 by Nils Steenbock
//*
//* DESCRIPTION  : set directory flag in TOC_Entry
//*
//* INPUT        : index - index of TOC_Entry
//*                dirf - dirflag
//*
//* OUTPUT       : -
//*
//***********************************************************************

void TOC_setDir(int index, u08 dirf)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) toc1[index].dirflag=dirf;
	if (arr==1) toc2[index].dirflag=dirf;
	if (arr==2) toc3[index].dirflag=dirf;

}


//***********************************************************************
//*
//* FUNCTION	 : TOC_setLevel
//*
//* CREATED	 : 09.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.12.2000 by Nils Steenbock
//*
//* DESCRIPTION  : set directory level in TOC_Entry
//*
//* INPUT        : index - index of TOC_Entry
//*                level - directory level
//*
//* OUTPUT       : -
//*
//***********************************************************************

void TOC_setLevel(int index,u08 level)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) toc1[index].level=level;
	if (arr==1) toc2[index].level=level;
	if (arr==2) toc3[index].level=level;

}


//***********************************************************************
//*
//* FUNCTION	 : TOC_setTitleChar
//*
//* CREATED	 : 09.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.12.2000 by Nils Steenbock
//*
//* DESCRIPTION  : set character in title of in TOC_Entry
//*
//* INPUT        : index - index of TOC_Entry
//*                ch - Character
//*
//* OUTPUT       : -
//*
//***********************************************************************


void TOC_setTitleChar(int index, u08 offset,u08 ch)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) toc1[index].title[offset]=ch;
	if (arr==1) toc2[index].title[offset]=ch;
	if (arr==2) toc3[index].title[offset]=ch;

}


//***********************************************************************
//*
//* FUNCTION	 : TOC_setStartsector
//*
//* CREATED	 : 09.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.12.2000 by Nils Steenbock
//*
//* DESCRIPTION  : set byte of startsector in TOC_Entry
//*
//* INPUT        : index - index of TOC_Entry
//*                ch - Byte
//*
//* OUTPUT       : -
//*
//***********************************************************************

void TOC_setStartsector(int index, u08 offset,u08 ch)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) toc1[index].startsector[offset]=ch;
	if (arr==1) toc2[index].startsector[offset]=ch;
	if (arr==2) toc3[index].startsector[offset]=ch;

}


//***********************************************************************
//*
//* FUNCTION	 : TOC_setLength
//*
//* CREATED	 : 09.12.2000 by Nils Steenbock
//*
//* LAST CHANGED : 09.12.2000 by Nils Steenbock
//*
//* DESCRIPTION  : set byte of length in TOC_Entry
//*
//* INPUT        : index - index of TOC_Entry
//*                ch - Byte
//*
//* OUTPUT       : -
//*
//***********************************************************************


void TOC_setLength(int index, u08 offset, u08 ch)
{
	u08 arr=0;
	for(;index>99;arr++) index-=100;
	if (arr==0) toc1[index].length[offset]=ch;
	if (arr==1) toc2[index].length[offset]=ch;
	if (arr==2) toc3[index].length[offset]=ch;

}

//***********************************************************************
//*
//* $Log: toc.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
