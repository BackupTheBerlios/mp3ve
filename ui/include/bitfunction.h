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
//* $Id: bitfunction.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : bitfunction header file
//*  Version     : 1.00
//*
//*  Filename    : bitfunction.h
//*  Created     : 16.09.2000 by Thomas Gutzeit
//*  Last changed: 16.09.2000 by Thomas Gutzeit
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3-IO-Interface
//*  Description : header for bitfunction.c
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : -
//*
//***********************************************************************

#ifndef __bitfunction_h__
#define __bitfunction_h__


typedef unsigned char u08;

u08 set_bit(u08 regi, u08 bit);

u08 clear_bit(u08 regi, u08 bit);

u08 bit_set(u08 regi, u08 bit);

u08 convert_to_hex_h(u08 temp);
u08 convert_to_hex_l(u08 temp);



#endif


//***********************************************************************
//*
//* $Log: bitfunction.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
