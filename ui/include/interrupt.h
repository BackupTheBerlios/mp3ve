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
//* $Id: interrupt.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : interrupt
//*  Version     : 1.5
//*
//*  Filename    : signal.h
//*  Created     : 21.12.2000 by unknowm
//*  Last changed: 21.12.2000 by Marc Dressler
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : -
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : ?
//*
//***********************************************************************

#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include <io.h>
#include "bitfunction.h"

#define sei()  asm volatile ("sei" ::)
#define cli()  asm volatile ("cli" ::)

extern inline void enable_external_int (unsigned char ints)
{
#ifdef AVR_MEGA
  outp (ints, EIMSK);
#else
  outp (ints, GIMSK);
#endif
}

extern inline void timer_enable_int (unsigned char ints)
{
  outp (ints, TIMSK);
}

#endif

//***********************************************************************
//*
//* $Log: interrupt.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
