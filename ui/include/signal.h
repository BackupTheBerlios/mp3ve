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
//* $Id: signal.h,v 1.1 2002/02/24 11:52:26 dressler Exp $
//***********************************************************************
//*
//*  Title     	 : Signal
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


#ifndef _SIGNAL_H_
#define _SIGNAL_H_

/* External Interrupt0 Function name */
#define SIG_INTERRUPT0 _interrupt0_

/* External Interrupt1 Function name */
#define	SIG_INTERRUPT1 _interrupt1_

/* External Interrupt2 Function name (ATmega) */
#define	SIG_INTERRUPT2 _interrupt2_

/* External Interrupt3 Function name (ATmega[16]03)*/
#define	SIG_INTERRUPT3 _interrupt3_

/* External Interrupt4 Function name (ATmega[16]03) */
#define	SIG_INTERRUPT4 _interrupt4_

/* External Interrupt5 Function name (ATmega[16]03) */
#define	SIG_INTERRUPT5 _interrupt5_

/* External Interrupt6 Function name (ATmega[16]03) */
#define	SIG_INTERRUPT6 _interrupt6_

/* External Interrupt7 Function name (ATmega[16]03) */
#define	SIG_INTERRUPT7 _interrupt7_

/* Output Compare2 Interrupt Function name */
#define SIG_OUTPUT_COMPARE2 _output_compare2_

/* Overflow2 Interrupt Function name */
#define	SIG_OVERFLOW2 _overflow2_

/* Input Capture1 Interrupt Function name */
#define SIG_INPUT_CAPTURE1 _input_capture1_

/* Output Compare1(A) Interrupt Function name */
#define SIG_OUTPUT_COMPARE1A _output_compare1a_

/* Output Compare1B Interrupt Function name */
#define	SIG_OUTPUT_COMPARE1B _output_compare1b_

/* Overflow1 Interrupt Function name */
#define	SIG_OVERFLOW1 _overflow1_

/* Output Compare0 Interrupt Function name */
#define SIG_OUTPUT_COMPARE0 _output_compare0_

/* Overflow0 Interrupt Function name */
#define	SIG_OVERFLOW0 _overflow0_

/* SPI Interrupt Function name */
#define	SIG_SPI _spi_

/* UART(0) Receive Complete Interrupt Function name */
#define	SIG_UART_RECV _uart_recv_

/* UART1 Receive Complete Interrupt Function name (ATmega161) */
#define	SIG_UART1_RECV _uart1_recv_

/* UART(0) Data Register Empty Interrupt Function name */
#define	SIG_UART_DATA _uart_data_

/* UART1 Data Register Empty Interrupt Function name (ATmega161) */
#define	SIG_UART1_DATA _uart1_data_

/* UART(0) Transmit Complete Interrupt Function name */
#define	SIG_UART_TRANS _uart_trans_

/* UART1 Transmit Complete Interrupt Function name (ATmega161) */
#define	SIG_UART1_TRANS _uart1_trans_

/* ADC Conversion complete */
#define	SIG_ADC _adc_

/* Eeprom ready */
#define	SIG_EEPROM _eeprom_ready_

/* Analog Comparator Interrupt Function name */
#define	SIG_COMPARATOR _comparator_

#define SIGNAL(signame)					\
void signame (void) __attribute__ ((signal));		\
void signame (void)

#define INTERRUPT(signame)				\
void signame (void) __attribute__ ((interrupt));	\
void signame (void)

#endif /* _SIGNAL_H_ */

//***********************************************************************
//*
//* $Log: signal.h,v $
//* Revision 1.1  2002/02/24 11:52:26  dressler
//* initial checkin
//*
