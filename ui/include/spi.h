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
//* $Id: spi.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title       : SPI Header-File
//*  Version     : 1.5
//*
//*  Filename    : spi.h
//*  Created     : 10.05.2000 by Thomas Gutzeit
//*  Last changed: 21.12.2000 by Marc Dressler
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : header-file for spi.c
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : io.h
//*                bitfunction.h
//*
//***********************************************************************

#include <io.h>
#include "bitfunction.h"


#ifndef __SPI_H__
#define __SPI_H__




void SPI_Init (void);

void SPI_Wait (void);

u08 SPI_Write	(u08 param1);

void SPI_Clk (void);

u08 SPI_Cycle (u08 led_param, u08 lcd_param);




#endif

//***********************************************************************
//*
//* $Log: spi.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
