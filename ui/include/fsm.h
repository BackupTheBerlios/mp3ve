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
//* $Id: fsm.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : fsm header file
//*  Version     : 1.5
//*
//*  Filename    : fsm.h
//*  Created     : 12.09.2000 by Thomas Gutzeit
//*  Last changed: 07.01.2001 by Thomas Gutzeit
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : header for fsm.c
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : progmem.h, lcd.h, mp3.h, interrupt.h
//*
//***********************************************************************



#include <progmem.h>
#include "lcd.h"
#include "mp3.h"
#include "dev.h"
#include "interrupt.h"
#include "sys.h"

#ifndef __automat_h__
#define __automat_h__

//FSM_flag_current//
#define error 7
#define ide_alive 6
#define master_slave 5
#define pausing_bit 4
#define medium_changed 3
#define next_song 2
#define left_bit 1
#define right_bit 0

//FSM_play_control//
#define	 play_all 0
#define play_dir 1
#define play_current_dir 2
#define od 3
#define shift_control 4

int FSM_next_toc_playing;		//the next song that is going to play (found out e.g. in state 99)
int FSM_current_toc_entry;		//the current playing title
int FSM_next_toc_entry;			//the next song that is going to play (found out in select-mode)
int FSM_start_toc_entry;
int FSM_max_toc_entry;
int FSM_last_toc_entry;			//only used in state 54
int FSM_next_level;


u08 FSM_start_level;
u08 drive_info;
int dead;



int j;

u08 CMD_RX;
u08 FSM_state, FSM_flag_current, FSM_flag_current_new, FSM_play_control, FSM_state_last;
u08 FSM_sig, FSM_sig_old;


u08 FSM_fll;	//first line length of the current title written to LCD
u08 FSM_sll;	//second line length
u08 FSM_shift_length;
u08 FSM_shift_right;
u08 FSM_shifts; //number of shifts done
int time, time2;

int FSM_line_no;

/*
key																flag
80			40		20		10		08		04	02	01			80		40		20		10		08		04		02		01
D-			MODE	frei	PAUSE	STOP	<<	>>	PLAY		error	ide_ali	frei	paused	medium	next	rechts	links
Taster																							_changed_song

*/


static const struct  Line {	u08 FSM_state;	u08 key_pattern;	u08 key_mask;	u08 flag_pattern;	u08 flag_mask;	u08 next_FSM_state;} __attribute__ ((progmem))
Automat [] = {{ 10,			0x00,				0x00,				0x40,			0x40,			11},
			  {	11,			0x04,				0xff,				0x00,			0x00,			12},
			  { 11,			0x02,				0xff,				0x00,			0x00,			13},
			  {	11,			0x00,				0x00,				0x08,			0x08,			11},
			  {	12,			0x00,				0x00,				0x00,			0x00,			52},
			  {	13,			0x00,				0x00,				0x00,			0x00,			52},

	          { 52,			0x00,				0x00,				0x80,			0x80,			10},
	          { 52,			0x00,				0x00,				0x04,			0x04,			99},
			  {	52,			0x00,				0x00,				0x08,			0x08,			11},
	          { 52,			0x00,				0x00,				0x00,			0x00,			54},


	          { 53,			0x00,				0x00,				0x00,			0x00,			59},
	          { 53,			0x00,				0x00,				0x04,			0x04,			99},
			  {	53,			0x00,				0x00,				0x08,			0x08,			11},

	          { 54,			0x00,				0x00,				0x01,			0x01,			54},
	          { 54,			0x00,				0x00,				0x02,			0x02,			55},
	          { 54,			0x02,				0xff,				0x00,			0x00,			99},
	          { 54,			0x01,				0xff,				0x00,			0x00,			53},
	          { 54,			0x04,				0xff,				0x00,			0x00,			57},
			  { 54,			0x80,				0xff,				0x00,			0x00,			58},
	          { 54,			0x40,				0xff,				0x00,			0x00,			59},
	          { 54,			0x08,				0xff,				0x00,			0x00,			61},
	          { 54,			0x10,				0xff,				0x00,			0x00,			60},
	          { 54,			0x00,				0x00,				0x04,			0x04,			99},
			  {	54,			0x00,				0x00,				0x08,			0x08,			11},

	          { 55,			0x00,				0x00,				0x02,			0x02,			55},
	          { 55,			0x00,				0x00,				0x01,			0x01,			54},
	          { 55,			0x02,				0xff,				0x00,			0x00,			99},
    	   	  { 55,			0x01,				0xff,				0x00,			0x00,			53},
  	          { 55,			0x04,				0xff,				0x00,			0x00,			57},
	   		  { 55,			0x80,				0xff,				0x00,			0x00,			58},
	 	      { 55,			0x40,				0xff,				0x00,			0x00,			59},
	          { 55,			0x08,				0xff,				0x00,			0x00,			61},
	          { 55,			0x10,				0xff,				0x00,			0x00,			60},
	          { 55,			0x00,				0x00,				0x04,			0x04,			99},
			  {	55,			0x00,				0x00,				0x08,			0x08,			11},

	          { 56,			0x00,				0x00,				0x00,			0x00,			59},
	          { 56,			0x00,				0x00,				0x04,			0x04,			99},
			  {	56,			0x00,				0x00,				0x08,			0x08,			11},

	          { 57,			0x00,				0x00,				0x00,			0x00,			59},
	          { 57,			0x00,				0x00,				0x04,			0x04,			99},
			  {	57,			0x00,				0x00,				0x08,			0x08,			11},

	          { 58,			0x00,				0x00,				0x01,			0x01,			54},
			  { 58,			0x00,				0x00,				0x02,			0x02,			55},
			  { 58,			0x01,				0xff,				0x00,			0x00,			53},
			  { 58,			0x00,				0x00,				0x04,			0x04,			99},
			  {	58,			0x00,				0x00,				0x08,			0x08,			11},

			  { 59,			0x00,				0xff,				0x01,			0x11,			54},
			  { 59,			0x10,				0xff,				0x00,			0x00,			60},
			  { 59,			0x08,				0xff,				0x00,			0x00,			61},
			  { 59,			0x00,				0x00,				0x02,			0x12,			55},
			  { 59,			0x01,				0xff,				0x10,			0x10,			60},
			  { 59,			0x04,				0xff,				0x00,			0x00,			57},
			  { 59,			0x02,				0xff,				0x00,			0x00,			99},
			  { 59,			0x00,				0x00,				0x04,			0x14,			99},
			  {	59,			0x00,				0x00,				0x08,			0x08,			11},

			  { 60,			0x00,				0x00,				0x00,			0x00,			59},
			  { 60,			0x00,				0x00,				0x04,			0x04,			99},
			  {	60,			0x00,				0x00,				0x08,			0x08,			11},

			  { 61,			0x01,				0xff,				0x00,			0x00,			53},
			  { 61,			0x00,				0x00,				0x01,			0x01,			54},
			  { 61,			0x00,				0x00,				0x02,			0x02,			55},
			  { 61,			0x00,				0x00,				0x04,			0x04,			99},
			  {	61,			0x00,				0x00,				0x08,			0x08,			11}
			};




void FSM_Title_out(int title_no);

void FSM_State_op(u08 FSM_state);

void FSM_Find_next_FSM_state (void);

void FSM_check (void);

void FSM_Init(void);

void FSM_Scroll_Title(void);



#endif


//***********************************************************************
//*
//* $Log: fsm.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
