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
//* $Id: fsm.c,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title       : fsm
//*  Version     : 1.5
//*
//*  Filename    : fsm.c
//*  Created     : 12.09.2000 by Thomas Gutzeit
//*  Last changed: 03.03.2001 by Thomas Gutzeit
//*
//*  Target-uC   : Atmel AVR AT90S8515
//*  Target-PCB  : MP3CtrlIO15
//*  Description : definition and basic functions to run the fsm
//*
//*  Homepage    : www.MP3ve.de
//*                www.uC-Elektronik.de
//*                www.Hardware-Schmiede.de
//*
//*  Uses        : fsm.h
//*
//***********************************************************************



#include "fsm.h"



//***********************************************************************
//*
//* FUNCTION     : FSM_State_op
//*
//* CREATED      : 12.09.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 03.03.2001 by Thomas Gutzeit
//*
//* DESCRIPTION  : decide what to do in the current state
//*
//* INPUT        : current_FSM_state
//*
//* OUTPUT       : -
//*
//***********************************************************************

void
FSM_State_op (u08 current_FSM_state)
{
  switch (current_FSM_state)
    {

	case 10:
	  dead=0;
	  LCD_Clear();
	  LCD_WriteTxt("   MP3ve v1.5");
	  LCD_SetCursor(lcd_line_no2);
	  LCD_WriteTxt("  www.mp3ve.de");
	  while ((!bit_set(FSM_flag_current_new,ide_alive)) & (dead<30))
	  {
		  SYS_Ping();
		  dead++;
	  	  LCD_Wait(100);
	  }
	  if (dead==30)
	  	{

			LCD_Clear();
	  		LCD_WriteTxt("please reset!");
		}
	  if (bit_set(FSM_flag_current_new,ide_alive))
	  	{
			LCD_Clear();
			LCD_WriteTxt("weiter mit Taste...");
		}
	  break;


    case 11:
      FSM_play_control = clear_bit (FSM_play_control, shift_control);	//in this state no shifting
      LCD_Clear ();

      FSM_flag_current = 0x00;
	  FSM_flag_current_new = 0x00;
	  FSM_current_toc_entry = 0;
	  FSM_next_toc_entry = 0;
  	  FSM_max_toc_entry = 0;
  	  FSM_next_toc_playing = 0;

      drive_info=DEV_Get_Info();
      if ((drive_info & 0x04)==0x04)
      {	if ((drive_info & 0x40)==0x40)
      	   LCD_WriteTxt("M: HD1 |");
      	else
      	   LCD_WriteTxt("M: CD1 |");
   	  	LCD_SetCursor(lcd_line_no2);
   	  	LCD_WriteTxt("<<     |");
	  }
      if ((drive_info & 0x08)==0x08)
      { LCD_SetCursor(0x08);
      	if ((drive_info & 0x80)==0x80)
     		LCD_WriteTxt("|S: HD2");
     	else
     		LCD_WriteTxt("|S: CD2");
   	  	LCD_SetCursor(0x30);
   	  	LCD_WriteTxt("|     >>");

	  }
  	  break;

    case 12:
      FSM_flag_current_new = set_bit (FSM_flag_current_new, master_slave);
	  break;

	case 13:
	  FSM_flag_current_new = clear_bit (FSM_flag_current_new, master_slave);
	  break;

    case 52:
      LCD_Clear ();
      LCD_WriteTxt ("Bitte Warten...");
      LCD_SetCursor (lcd_line_no2);
      //LCD_Wait(180);                        //wait until cd-rom is ready
      LCD_WriteTxt ("Lese Toc...");

      if (bit_set(FSM_flag_current_new, master_slave))
	DEV_Select_Master();
      else DEV_Select_Slave();

      LCD_Wait(100);

      FSM_max_toc_entry = TOC_Read ();
      if (FSM_max_toc_entry==0)
      {
		  LCD_Clear();
		  LCD_WriteTxt("keine CD?");
		  FSM_flag_current_new = set_bit (FSM_flag_current_new, error);
	  }
      //LCD_SetCursor (lcd_line_no2);
      else
      {
		  FSM_next_toc_entry = -1;
          LCD_WriteTxt ("OK!");
	  }
      break;

    case 53:
      LCD_Clear ();


      FSM_start_level = FSM_next_level;
      if (!(TOC_isDir (FSM_next_toc_entry))
	  && (!bit_set (FSM_play_control, od)))
	{
	  FSM_play_control = 0x00;
	  FSM_play_control = set_bit (FSM_play_control, play_current_dir);
	}
      else
	{
	  if (bit_set (FSM_play_control, od))
	    {
	      if (FSM_next_toc_entry > 0)
		{
		  FSM_play_control = clear_bit (FSM_play_control, od);
		  break;
		}
	      else
		{
		  FSM_play_control = 0x00;
		  FSM_play_control = set_bit (FSM_play_control, play_all);
		  do
		    {
		      FSM_next_toc_entry++;
		    }
		  while (TOC_isDir (FSM_next_toc_entry));
		}
	    }
	  else
	    {
	      FSM_play_control = 0x00;
	      FSM_play_control = set_bit (FSM_play_control, play_dir);
	      do
		{
		  FSM_next_toc_entry++;
		}
	      while (TOC_isDir (FSM_next_toc_entry)
		     && (FSM_next_toc_entry < (FSM_max_toc_entry - 1)));
	      if (TOC_isDir (FSM_next_toc_entry))
		break;

	    }
	}

      MP3_Next (FSM_next_toc_entry);
      MP3_Play ();
      FSM_current_toc_entry = FSM_next_toc_entry;
      FSM_start_toc_entry = FSM_next_toc_entry;
      FSM_next_toc_playing = FSM_next_toc_entry;
      FSM_next_level = TOC_getLevel (FSM_next_toc_entry);
      led_state = FSM_play_control;
      break;



    case 54:
      LCD_Clear ();
      FSM_play_control = set_bit (FSM_play_control, shift_control);
      FSM_last_toc_entry = FSM_next_toc_entry;
      if (bit_set (FSM_play_control, od))
	goto ok;
      else
	{
	  if (FSM_next_toc_entry == (FSM_max_toc_entry - 1))
	    goto ok;
	  else
	    {
	      do
		{
		  FSM_next_toc_entry++;
		}
	      while ((TOC_getLevel (FSM_next_toc_entry) > FSM_next_level)
		     && (FSM_next_toc_entry < (FSM_max_toc_entry - 1)));
	      if (TOC_getLevel (FSM_next_toc_entry) == FSM_next_level)
		goto ok;
	      else
		FSM_next_toc_entry = FSM_last_toc_entry;
	    }
	}
    ok:FSM_Title_out (FSM_next_toc_entry);
      FSM_play_control = clear_bit (FSM_play_control, od);
      break;



    case 55:
      LCD_Clear ();
      FSM_play_control = set_bit (FSM_play_control, shift_control);
      if ((bit_set (FSM_play_control, od)) && (FSM_next_toc_entry > 0))
	{
	  LCD_WriteTxt ("..");
	  break;
	}
      else if (FSM_next_toc_entry > 0)
	{

	  do
	    {
	      FSM_next_toc_entry--;
	    }
	  while ((TOC_getLevel (FSM_next_toc_entry) > FSM_next_level)
		 && (FSM_next_toc_entry > 0));
	  if (TOC_getLevel (FSM_next_toc_entry) != FSM_next_level)
	    {
	      LCD_WriteTxt ("..");
	      FSM_next_toc_entry++;
	      FSM_play_control = set_bit (FSM_play_control, od);
	      break;
	    }

	}
      else //if (FSM_next_toc_entry == 0)
	{
	  LCD_WriteTxt ("root");
	  FSM_next_toc_entry=0;
	  FSM_play_control = set_bit (FSM_play_control, od);
	  break;

	}

      FSM_Title_out (FSM_next_toc_entry);
      break;


    case 57:
      LCD_Clear ();
      if (FSM_current_toc_entry > 0)
	FSM_current_toc_entry--;
      while ((FSM_current_toc_entry > 0)
	     && (TOC_isDir (FSM_current_toc_entry)))
	FSM_current_toc_entry--;
      if (bit_set (FSM_flag_current_new, pausing_bit))
	{
	  MP3_Play ();
	  FSM_flag_current_new =
	    clear_bit (FSM_flag_current_new, pausing_bit);
	}
      MP3_Next (FSM_current_toc_entry);
 MP3_Play();
      FSM_next_toc_entry = FSM_current_toc_entry;
      FSM_next_toc_playing = FSM_next_toc_entry;
      break;

    case 58:
      FSM_state = FSM_state_last;
      if ((bit_set (FSM_play_control, od)) && (FSM_next_toc_entry > 0))
	{
	  FSM_next_level--;
	  FSM_next_toc_entry--;
	  FSM_play_control = clear_bit (FSM_play_control, od);
	}
      else
	{
	  if (!(TOC_isDir (FSM_next_toc_entry))
	      || ((FSM_next_toc_entry == 0) &
		  (bit_set (FSM_play_control, od))))

	    break;
	  else
	    {
	      FSM_next_level++;
	      FSM_next_toc_entry++;
	    }
	}

      FSM_Title_out (FSM_next_toc_entry);

      break;



    case 59:
      if (!(bit_set (FSM_flag_current_new, pausing_bit)))
	{
	  FSM_Title_out (FSM_current_toc_entry);
	  FSM_next_toc_entry = FSM_current_toc_entry;
	  FSM_next_level = TOC_getLevel (FSM_current_toc_entry);
	  FSM_play_control = clear_bit (FSM_play_control, od);
	  FSM_play_control = set_bit (FSM_play_control, shift_control);
	}
      else
	{
	  LCD_Clear ();
	  LCD_WriteTxt ("PAUSE");
	}
      break;

    case 60:
      FSM_play_control = clear_bit (FSM_play_control, shift_control);	//in this state no shifting
      LCD_Clear ();
      if (bit_set (FSM_flag_current_new, pausing_bit))
	{
	  FSM_flag_current_new =
	    clear_bit (FSM_flag_current_new, pausing_bit);
	  MP3_Play ();
	}
      else
	{
	  FSM_flag_current_new = set_bit (FSM_flag_current_new, pausing_bit);
	  MP3_Pause ();
	  LCD_WriteTxt ("PAUSE");
	};
      break;

    case 61:
      MP3_Stop ();
      //FSM_next_toc_entry = -1;
      //FSM_current_toc_entry = 0;
      //FSM_next_level = 0;
      FSM_play_control = clear_bit (FSM_play_control, shift_control);	//in this state no shifting
      FSM_flag_current_new = clear_bit (FSM_flag_current_new, pausing_bit);
      LCD_Clear ();
      LCD_WriteTxt ("STOP");

      break;

    case 99:
      FSM_next_toc_playing++;
      if (FSM_next_toc_playing > (FSM_max_toc_entry - 1))
	FSM_next_toc_playing = FSM_start_toc_entry;

      else if (bit_set (FSM_play_control, play_all))
	{
	  while (TOC_isDir (FSM_next_toc_playing))
	    FSM_next_toc_playing++;
	  if (FSM_next_toc_playing > (FSM_max_toc_entry - 1))
	    FSM_next_toc_playing = FSM_start_toc_entry;
	}
      else if (bit_set (FSM_play_control, play_dir))
	{
	  if (TOC_getLevel (FSM_next_toc_playing) < FSM_start_level + 1)
	    FSM_next_toc_playing = FSM_start_toc_entry;
	  else
	    {
	      while (TOC_isDir (FSM_next_toc_playing) &
		     (TOC_getLevel (FSM_next_toc_playing) > FSM_start_level) &
		     (FSM_next_toc_playing < (FSM_max_toc_entry - 1)))
		{
		  FSM_next_toc_playing++;
		}
	    }
	}
      else if (bit_set (FSM_play_control, play_current_dir))
	{
	  if (!(TOC_getLevel (FSM_next_toc_playing) == FSM_start_level))
	    FSM_next_toc_playing = FSM_start_toc_entry;
	}

      MP3_Next (FSM_next_toc_playing);
MP3_Play();
      FSM_current_toc_entry = FSM_next_toc_playing;
      if (FSM_state_last == 59)
	FSM_Title_out (FSM_current_toc_entry);


      FSM_state = FSM_state_last;
      FSM_flag_current_new = clear_bit (FSM_flag_current_new, next_song);
      break;



    }				//of switch

}				//of find next FSM_state



//***********************************************************************
//*
//* FUNCTION     : FSM_Title_out
//*
//* CREATED      : 15.10.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 07.12.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : write the current toc-entry to display
//*
//* INPUT        : title_no
//*
//* OUTPUT       : -
//*
//***********************************************************************

void FSM_Title_out (int title_no)
{
  u08 temp;
  u08 temp2;
  u08 temp3;
  u08 temp4;
  u08 temp5;

  temp2 = 30;
  temp3 = 0;
  temp4 = 1;
  temp5 = 0;

  FSM_fll = FSM_sll = 0;

  LCD_Clear ();

  if ((TOC_getTitleChar (title_no, 0)) < 58)	//if first character is a number
    temp4 = 2;

  for (j = 0; j < 40; j++)
    {
      temp = TOC_getTitleChar (title_no, j);
      if (temp == 0x5f)
	LCD_Write (0x20);	//if "_" write a blank instead
      else if (!(temp == 46))
	LCD_Write (temp);
      if (temp == 0x20)
	temp3++;		//if three successive blanks then stop writing
      else
	temp3 = 0;
      if (((j > 5) && (temp == 46)) || (temp3 == 3))	//if "." or the third blank
	{
	  FSM_fll = j - temp3;
	  j = 60;
	  temp2 = j;
	}
      if (temp == 45)		//if "-" then go on writing in next line
	{
	  temp5++;
	  if (temp5 == temp4)
	    {
	      FSM_fll = j;	//length of the first line
	      temp2 = j + 1;
	      j = 40;		//stops the for-loop
	    }
	}
      else if (j == 39)
	{
	  FSM_fll = j;
	  temp2 = j + 1;
	}
    }



  LCD_SetCursor (lcd_line_no2);
  for (j = temp2; j < 60; j++)
    {
      temp = TOC_getTitleChar (title_no, j);
      if (temp == 0x5f)
	LCD_Write (0x20);	//if "_" write a blank instead
      else if (!(temp == 46))
	LCD_Write (temp);
      if (temp == 0x20)
	temp3++;		//if three successive blanks then stop writing
      else
	temp3 = 0;
      if ((temp == 46) || (temp3 == 3))	//if "." or the third blank
	{
	  FSM_sll = j - temp2 - temp3;
	  j = 60;
	}
      else if ((j - temp2) == 39)
	{
	  FSM_sll = j - temp2;
	  j = 60;
	}
    }


  if (FSM_fll > FSM_sll)
    FSM_shift_length = FSM_fll;
  else
    FSM_shift_length = FSM_sll;
  if (FSM_shift_length <= 16)
    FSM_shift_length = 0;
  else
    FSM_shift_length = FSM_shift_length - 15;
  FSM_shift_right = 1;
  FSM_shifts = 0;
}


//***********************************************************************
//*
//* FUNCTION     : FSM_Find_next_FSM_state
//*
//* CREATED      : 12.09.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 03.03.2001 by Thomas Gutzeit
//*
//* DESCRIPTION  : looking through the fsm-table to find the next state
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void
FSM_Find_next_FSM_state (void)
{
  u08 key_mask;
  u08 flag_mask;
  u08 key_pattern;
  u08 flag_pattern;


  FSM_line_no = 0;
  for (FSM_line_no = 0; FSM_line_no < 63; FSM_line_no++)
    {
      key_mask = PRG_RDB (&Automat[FSM_line_no].key_mask);
      flag_mask = PRG_RDB (&Automat[FSM_line_no].flag_mask);
      key_pattern = PRG_RDB (&Automat[FSM_line_no].key_pattern);
      flag_pattern = PRG_RDB (&Automat[FSM_line_no].flag_pattern);

      if (PRG_RDB (&Automat[FSM_line_no].FSM_state) == FSM_state)
	{
	  if ((key_state_new & key_mask) == (key_pattern & key_mask))
	    {
	      if ((FSM_flag_current_new & flag_mask) ==
		  (flag_pattern & flag_mask))
		{
		  FSM_state_last = FSM_state;
		  FSM_state = PRG_RDB (&Automat[FSM_line_no].next_FSM_state);
		  FSM_State_op (FSM_state);
		  led_state = FSM_play_control;
		  break;
		}
	    }
	}
    }				//of for
}				//of FSM_Find_next_FSM_state





//***********************************************************************
//*
//* FUNCTION     : FSM_Init
//*
//* CREATED              : 10.11.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 30.11.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : Initialize the state machine
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************


void
FSM_Init (void)
{
  time = time2 = 0;
  FSM_state = 10;
  FSM_line_no = 0;
  CMD_RX = 0;
  FSM_flag_current = 0x00;
  FSM_flag_current_new = 0x00;
  FSM_current_toc_entry = 0;
  FSM_next_toc_entry = 0;
  FSM_max_toc_entry = 0;
  key_state = 0x00;
  led_state = 0x00;

}




//***********************************************************************
//*
//* FUNCTION     : FSM_Scroll_Title(void)
//*
//* CREATED              : 10.11.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 30.11.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : control the display-scrolling
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************


void
FSM_Scroll_Title (void)
{
  if ((time == 4000) && (!bit_set (FSM_play_control, od))
      && (bit_set (FSM_play_control, shift_control)))
    {
      if ((FSM_shifts == FSM_shift_length) || (FSM_shifts == 0))
	{
	  if (time2 < 3)
	    {
	      time2++;
	      time = 0;
	      goto end;
	    }
	  else
	    time2 = 0;
	}


      if ((FSM_shift_right) && (FSM_shifts < FSM_shift_length))
	{
	  LCD_DisplayLeft ();
	  FSM_shifts++;
	}
      else
	{
	  FSM_shift_right = 0;

	  if (FSM_shifts > 0)
	    {
	      LCD_DisplayRight ();
	      FSM_shifts--;
	    }
	  else
	    FSM_shift_right = 1;
	}
      time = 0;
    }
  else
    time++;

end:
  key_state = key_state_new;
  FSM_flag_current_new = clear_bit (FSM_flag_current_new, right_bit);
  FSM_flag_current_new = clear_bit (FSM_flag_current_new, left_bit);
  FSM_flag_current = FSM_flag_current_new;

}




//***********************************************************************
//*
//* FUNCTION     : FSM_check
//*
//* CREATED              : 12.09.2000 by Thomas Gutzeit
//*
//* LAST CHANGED : 30.11.2000 by Thomas Gutzeit
//*
//* DESCRIPTION  : analyze signals on int0 and int1 to determine
//*                                the direction of the rotary encoder
//*
//* INPUT        : -
//*
//* OUTPUT       : -
//*
//***********************************************************************

void
FSM_check (void)
{
  FSM_sig = inp (PIND);
  FSM_sig = FSM_sig / 4;
  FSM_sig = FSM_sig & (1 + 2);

  if ((FSM_sig_old == 1) && (FSM_sig == 3))
    {
      FSM_flag_current_new = set_bit (FSM_flag_current, 1);
    };
  if ((FSM_sig_old == 2) && (FSM_sig == 0))
    {
      FSM_flag_current_new = set_bit (FSM_flag_current, 1);
    };


  if ((FSM_sig_old == 0) && (FSM_sig == 2))
    {
      FSM_flag_current_new = set_bit (FSM_flag_current, 0);
    };
  if ((FSM_sig_old == 3) && (FSM_sig == 1))
    {
      FSM_flag_current_new = set_bit (FSM_flag_current, 0);
    };


  FSM_sig_old = FSM_sig;
}

//***********************************************************************
//*
//* $Log: fsm.c,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
