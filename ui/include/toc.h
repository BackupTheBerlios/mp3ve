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
//* $Id: toc.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//**********************************************************************
//*
//*  Title       : reading the TOC
//*  Version     : 1.5
//*
//*  Filename    : toc.h
//*  Created     : 11.09.2000 by Nils Steenbock
//*  Last changed: 21.12.2000 by Marc Dressler
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

#include "rpc.h"
#include <interrupt.h>

#ifndef __TOC_H__
#define __TOC_H__

typedef struct
{

  u08 level;
  u08 dirflag;
  u08 startsector[4];
  u08 length[4];
  u08 title[60];
} TOC_Entry;



int TOC_Read(void);

u08 TOC_isDir(int index);
u08 TOC_getLevel(int index);
u08 TOC_getTitleChar(int index, u08 offset);
u08 TOC_getStartsector(int index, u08 offset);
u08 TOC_getLength(int index, u08 offset);


void TOC_setDir(int index,u08 dirf);
void TOC_setLevel(int index,u08 level);
void TOC_setTitleChar(int index, u08 offset,u08 ch);
void TOC_setStartsector(int index, u08 offset,u08 ch);
void TOC_setLength(int index, u08 offset, u08 ch);


#endif

//***********************************************************************
//*
//* $Log: toc.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
