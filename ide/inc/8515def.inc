;***********************************************************************
;*  MP3VE - Standalone MP3 Player
;*  Copyright (C) 2000,2001,2002 Marc Dressler, Jens Funke,
;*                               Thomas Gutzeit, Nils Steenbock
;*
;*  This program is free software; you can redistribute it and/or modify
;*  it under the terms of the GNU General Public License as published by
;*  the Free Software Foundation; either version 2 of the License, or
;*  (at your option) any later version.
;*
;*  This program is distributed in the hope that it will be useful,
;*  but WITHOUT ANY WARRANTY; without even the implied warranty of
;*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;*  GNU General Public License for more details.
;*
;*  You should have received a copy of the GNU General Public License
;*  along with this program; if not, write to the Free Software
;*  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
;*
;*  info@mp3ve.de
;***********************************************************************

;***********************************************************************
;* $Id: 8515def.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Register/Bit Definitions for the AT90S8515	
;*  Version     : 1.5
;*				
;*  Filename    : 8515def.inc
;*  Created     : 12.12.1999 by Marc Dressler
;*  Last changed: 13.12.2000 by Marc Dressler
;*	
;*  Target-uC   : Atmel AVR AT90S8515
;*  Target-PCB  : all
;*  Description : additional registerdefinitions for AS
;*
;*  Uses        : -
;*
;***********************************************************************

	save
	listing off

        if       mompass=1
          message "8515 Additional Register Definitions V1.5 (C)2000 by Marc Dressler"
        endif


;* A P P L I C A T I O N   N O T E   F O R   T H E   A V R   F A M I L Y
;* 
;* Number		:AVR000
;* File Name		:"8515def.inc"
;* xTitle		:Register/Bit Definitions for the AT90S8515
;* Date                 :97.12.11
;* Version              :1.20
;* Support telephone	:+47 72 88 87 20 (ATMEL Norway)
;* Support fax		:+47 72 88 87 18 (ATMEL Norway)
;* Support E-mail	:avr@atmel.com
;* Target MCU		:AT90S8515
;*
;* DESCRIPTION
;* When including this file in the assembly program file, all I/O register	
;* names and I/O register bit names appearing in the data book can be used.
;* In addition, the six registers forming the three data pointers X, Y and
;* Z have been assigned names XL - ZH. Highest RAM address for Internal 
;* SRAM is also defined 
;*
;* The Register names are represented by their hexadecimal address.
;* 
;* The Register Bit names are represented by their bit number (0-7).
;* 
;* Please observe the difference in using the bit names with instructions
;* such as "sbr"/"cbr" (set/clear bit in register) and "sbrs"/"sbrc" 
;* (skip if bit in register set/cleared). The following example illustrates
;* this:
;* 
;* in	r16,PORTB		;read PORTB latch
;* sbr	r16,(1<<PB6)+(1<<PB5)	;set PB6 and PB5 (use masks, not bit#)
;* out  PORTB,r16		;output to PORTB
;*
;* in	r16,TIFR		;read the Timer Interrupt Flag Register
;* sbrc	r16,TOV0		;test the overflow flag (use bit#)
;* rjmp	TOV0_is_set		;jump if set
;* ...				;otherwise do something else

;***** Specify Device
;.device AT90S8515

;***** I/O Register Definitions
;.equ	SREG	=$3f
;.equ	SPH	=$3e
;.equ	SPL	=$3d
;.equ	GIMSK	=$3b
;.equ	GIFR	=$3a
;.equ	TIMSK	=$39
;.equ	TIFR	=$38
;.equ	MCUCR	=$35
;.equ	TCCR0	=$33
;.equ	TCNT0	=$32
;.equ	TCCR1A	=$2f
;.equ	TCCR1B	=$2e
;.equ	TCNT1H	=$2d
;.equ	TCNT1L	=$2c
;.equ	OCR1AH	=$2b
;.equ	OCR1AL	=$2a
;.equ	OCR1BH	=$29
;.equ	OCR1BL	=$28
;.equ	ICR1H	=$25
;.equ	ICR1L	=$24
;.equ	WDTCR	=$21
;.equ	EEARH	=$1f
;.equ	EEARL	=$1e
;.equ	EEDR	=$1d
;.equ	EECR	=$1c
;.equ	PORTA	=$1b
;.equ	DDRA	=$1a
;.equ	PINA	=$19
;.equ	PORTB	=$18
;.equ	DDRB	=$17
;.equ	PINB	=$16
;.equ	PORTC	=$15
;.equ	DDRC	=$14
;.equ	PINC	=$13
;.equ	PORTD	=$12
;.equ	DDRD	=$11
;.equ	PIND	=$10
;.equ	SPDR	=$0f
;.equ	SPSR	=$0e
;.equ	SPCR	=$0d
;.equ	UDR	=$0c
;.equ	USR	=$0b
;.equ	UCR	=$0a
;.equ	UBRR	=$09
;.equ	ACSR	=$08


;***** Bit Definitions
;.equ	INT1	=7
;.equ	INT0	=6

;.equ	TOIE1	=7
;.equ	OCIE1A	=6
;.equ	OCIE1B	=5
;.equ	TICIE1	=3
;.equ	TOIE0	=1

;.equ	TOV1	=7
;.equ	OCF1A	=6
;.equ	OCF1B	=5
;.equ	ICF1	=3
;.equ	TOV0	=1

;.equ	SRE	=7
;.equ	SRW	=6
;.equ	SE	=5
;.equ	SM	=4
;.equ	ISC11	=3
;.equ	ISC10	=2
;.equ	ISC01	=1
;.equ	ISC00	=0

;.equ	COM1A1	=7
;.equ	COM1A0	=6
;.equ	COM1B1	=5
;.equ	COM1B0	=4
;.equ	PWM11	=1
;.equ	PWM10	=0

;.equ	ICNC1	=7
;.equ	ICES1	=6
;.equ	CTC1	=3
;.equ	CS12	=2
;.equ	CS11	=1
;.equ	CS10	=0

;.equ	WDTOE	=4
;.equ	WDE	=3
;.equ	WDP2	=2
;.equ	WDP1	=1
;.equ	WDP0	=0

;.equ	EEMWE	=2
;.equ	EEWE	=1
;.equ	EERE	=0

PA7	EQU	7
PA6	EQU	6
PA5	EQU	5
PA4	EQU	4
PA3	EQU	3
PA2	EQU	2
PA1	EQU	1
PA0	EQU	0

DDA7	EQU	7
DDA6	EQU	6
DDA5	EQU	5
DDA4	EQU	4
DDA3	EQU	3
DDA2	EQU	2
DDA1	EQU	1
DDA0	EQU	0

PINA7	EQU	7
PINA6	EQU	6
PINA5	EQU	5
PINA4	EQU	4
PINA3	EQU	3
PINA2	EQU	2
PINA1	EQU	1
PINA0	EQU	0

PB7	EQU	7
PB6	EQU	6
PB5	EQU	5
PB4	EQU	4
PB3	EQU	3
PB2	EQU	2
PB1	EQU	1
PB0	EQU	0

DDB7	EQU	7
DDB6	EQU	6
DDB5	EQU	5
DDB4	EQU	4
DDB3	EQU	3
DDB2	EQU	2
DDB1	EQU	1
DDB0	EQU	0

PINB7	EQU	7
PINB6	EQU	6
PINB5	EQU	5
PINB4	EQU	4
PINB3	EQU	3
PINB2	EQU	2
PINB1	EQU	1
PINB0	EQU	0

PC7	EQU	7
PC6	EQU	6
PC5	EQU	5
PC4	EQU	4
PC3	EQU	3
PC2	EQU	2
PC1	EQU	1
PC0	EQU	0

DDC7	EQU	7
DDC6	EQU	6
DDC5	EQU	5
DDC4	EQU	4
DDC3	EQU	3
DDC2	EQU	2
DDC1	EQU	1
DDC0	EQU	0

PINC7	EQU	7
PINC6	EQU	6
PINC5	EQU	5
PINC4	EQU	4
PINC3	EQU	3
PINC2	EQU	2
PINC1	EQU	1
PINC0	EQU	0

PD7	EQU	7
PD6	EQU	6
PD5	EQU	5
PD4	EQU	4
PD3	EQU	3
PD2	EQU	2
PD1	EQU	1
PD0	EQU	0

DDD7	EQU	7
DDD6	EQU	6
DDD5	EQU	5
DDD4	EQU	4
DDD3	EQU	3
DDD2	EQU	2
DDD1	EQU	1
DDD0	EQU	0

PIND7	EQU	7
PIND6	EQU	6
PIND5	EQU	5
PIND4	EQU	4
PIND3	EQU	3
PIND2	EQU	2
PIND1	EQU	1
PIND0	EQU	0

;.equ	RXC	=7
;.equ	TXC	=6
;.equ	UDRE	=5
;.equ	FE	=4
;.equ	OR	=3

;.equ	SPIE	=7
;.equ	SPE	=6
;.equ	DORD	=5
;.equ	MSTR	=4
;.equ	CPOL	=3
;.equ	CPHA	=2
;.equ	SPR1	=1
;.equ	SPR0	=0

;.equ	SPIF	=7
;.equ	WCOL	=6

;.equ	RXCIE	=7
;.equ	TXCIE	=6
;.equ	UDRIE	=5
;.equ	RXEN	=4
;.equ	TXEN	=3
;.equ	CHR9	=2
;.equ	RXB8	=1
;.equ	TXB8	=0

;.equ	ACD	=7
;.equ	ACO	=5
;.equ	ACI	=4
;.equ	ACIE	=3
;.equ	ACIC	=2
;.equ	ACIS1	=1
;.equ	ACIS0	=0

;.def	XL	=r26
;.def	XH	=r27
;.def	YL	=r28
;.def	YH	=r29
;.def	ZL	=r30
;.def	ZH	=r31

;.equ 	RAMEND =$25F	;Last On-Chip SRAM Location

;.equ	INT0addr=$001	;External Interrupt0 Vector Address
;.equ	INT1addr=$002	;External Interrupt1 Vector Address
;.equ	ICP1addr=$003	;Input Capture1 Interrupt Vector Address
;.equ	OC1Aaddr=$004	;Output Compare1A Interrupt Vector Address
;.equ	OC1Baddr=$005	;Output Compare1B Interrupt Vector Address
;.equ	OVF1addr=$006	;Overflow1 Interrupt Vector Address
;.equ	OVF0addr=$007	;Overflow0 Interrupt Vector Address
;.equ	SPIaddr =$008	;SPI Interrupt Vector Address
;.equ	URXCaddr=$009	;UART Receive Complete Interrupt Vector Address
;.equ	UDREaddr=$00a	;UART Data Register Empty Interrupt Vector Address
;.equ	UTXCaddr=$00b	;UART Transmit Complete Interrupt Vector Address
;.equ	ACIaddr =$00c	;Analog Comparator Interrupt Vector Address


;***********************************************************************	
	listing on
	restore

;*
;* $Log: 8515def.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
