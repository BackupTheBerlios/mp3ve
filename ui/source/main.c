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
//* $Id: main.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : Main-file
//*  Version     : 1.5
//*
//*  Filename    : main.c
//*  Created     : 05.07.2000 by Thomas Gutzeit
//*  Last changed: 03.03.2001 by Thomas Gutzeit
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*  Target-PCB  : MP3CtrlIO15
//*
//*  Homepage    : www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//***********************************************************************

#include <io.h>
#include <progmem.h>
#include "interrupt.h"
#include "sig-avr.h"
#include "spi.h"
#include "lcd.h"
#include "toc.h"
#include "mp3.h"
#include "uart.h"
#include "toc.h"
#include "rpc.h"
#include "fsm.h"
#include "bitfunction.h"
#include "dev.h"


extern u08 lcd_state;
extern u08 led_state;
extern u08 key_state;
extern u08 key_state_new;
//extern TOC_Entry toc[117];

char ch;



SIGNAL(SIG_UART_RECV)	//signal handler for receive complete interrupt
{
  char led;
  cli();
  led = inp(UDR);        // read byte for UART data buffer
  if (led=='M')
    CMD_RX=1;
  if (led=='P')
    {
      FSM_flag_current_new=set_bit(FSM_flag_current_new,ide_alive);
    }
  sei();
}

int main (void)
{

	outp((1<<RXCIE),UCR);
	ch=inp(MCUCR);
	ch|=128;
	outp(ch,MCUCR);
	outp(0xFF,SPL);
	outp(0x7F,SPH);
	outp(0x00,DDRD);


	SPI_Init();
	LCD_Init();
	UART_Init();
	FSM_Init();

	DEV_Select_Master();
	FSM_State_op(FSM_state);
	for (;;)
	{
		FSM_check();
		led_state=FSM_play_control;
		key_state_new=~(SPI_Cycle(~led_state, lcd_state));

		if (CMD_RX==1)
			{
				CMD_RX=0;
				FSM_flag_current_new=set_bit(FSM_flag_current_new,next_song);
			}

		if ((key_state_new & (1<<5))==(1<<5)) FSM_flag_current_new=set_bit(FSM_flag_current_new,medium_changed);


		if ((key_state_new!=key_state) || (FSM_flag_current_new!=FSM_flag_current))
			FSM_Find_next_FSM_state();
		FSM_Scroll_Title();
		FSM_flag_current=FSM_flag_current_new;
	}

	return 0;

}


//***********************************************************************
//*
//* $Log: main.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
