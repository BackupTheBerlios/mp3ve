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
//* $Id: bitfunction.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	:  bitfunctions
//*  Version     : 1.00
//*
//*  Filename    : bitfunction.c
//*  Created     : 16.09.2000 by Thomas Gutzeit
//*  Last changed: 15.10.2000 by Thomas Gutzeit
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3-IO-Interface
//*  Description : some useful bit-manipulating functions
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : -
//*
//***********************************************************************


#include "bitfunction.h"

//***********************************************************************
//*
//* FUNCTION	 : set_bit
//*
//* CREATED		 : 16.09.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 15.10.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : set one bit
//*
//* INPUT        : u08 register, u08 bitnumber
//*
//* OUTPUT       : u08 register
//*
//***********************************************************************
u08 set_bit(u08 regi, u08 bit)
{
	return(regi | (1<<bit));
}


//***********************************************************************
//*
//* FUNCTION	 : clear_bit
//*
//* CREATED		 : 16.09.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 15.10.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : clear one bit
//*
//* INPUT        : u08 register, u08 bitnumber
//*
//* OUTPUT       : u08 register
//*
//***********************************************************************

u08 clear_bit(u08 regi, u08 bit)
{
	return (regi & (~(1<<bit)));
}


//***********************************************************************
//*
//* FUNCTION	 : bit_set
//*
//* CREATED		 : 16.09.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 15.10.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : check if one bit is set in register
//*
//* INPUT        : u08 register, u08 bitnumber
//*
//* OUTPUT       : u08 register
//*
//***********************************************************************

u08 bit_set(u08 regi, u08 bit)
{
	if ((regi & (1<<bit))==(1<<bit)) {return (1);}
	 else {return (0);};
}


//***********************************************************************
//*
//* FUNCTION	 : convert_to_hex_h / convert_to_hex_l
//*
//* CREATED		 : 16.09.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 15.10.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : used for give out u08 as hex on display
//*
//* INPUT        : u08 data
//*
//* OUTPUT       : u08 data
//*
//***********************************************************************


u08 convert_to_hex_h(u08 temp)
{
	temp=(temp & 0xf0);
	temp=temp/16;
	if (temp>=10)
		temp=temp+0x41;
	else temp=temp+0x30;
	return temp;
}

u08 convert_to_hex_l(u08 temp)
{
	temp=(temp & 0x0f);
	if (temp>=10)
		temp=temp+0x41;
	else temp=temp+0x30;
	return temp;
}


//***********************************************************************
//*
//* $Log: bitfunction.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
