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
;* $Id: timer.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title     	: Timer/Counter
;*  Version     : 1.5
;*
;*  Filename    : timer.inc
;*  Created     : 08.06.2000 by Jens Funke
;*  Last changed: 14.02.2001 by Marc Dressler
;*
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : MP3IDE V1.5
;*  Description : Timer/Counter Functions
;*
;*  Homepage    : www.MP3ve.de
;*                www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : -
;*
;***********************************************************************

     	ifndef  timerinc       ; verhindert Mehrfacheinbindung

timerinc       =     	1

		save
                listing off

                if      MOMPASS=1
                 message "Timer/Counter Functions (C) 2000 by Jens Funke"
		endif

        SEGMENT CODE
	
  ifdef all_code
;***********************************************************************
;* FUNCTION	: TIMER0_Enable_TOIE 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Enable Overflow Interrupt for TC0
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_Enable_TOIE:
       	push    temp1
	
       	in      temp1,TIMSK
       	sbr     temp1,(1<<TOIE0)
       	out     TIMSK,temp1
	
       	pop     temp1
       	ret
  
;***********************************************************************
;* FUNCTION	: TIMER1_Enable_TOIE
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Enable Overflow Interrupt for TC1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Enable_TOIE:
       	push    temp1
	
       	in      temp1,TIMSK
       	sbr     temp1,(1<<TOIE1)
       	out     TIMSK,temp1
	
       	pop     temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Enable_OCE1A 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Enable Output CompareA Match Interrupt for Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Enable_OCE1A:
       	push    temp1
	
       	in      temp1,TIMSK
       	sbr     temp1,(1<<OCIE1A)
       	out     TIMSK,temp1
	
       	pop     temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Enable_OCE1B
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Enable Output CompareB Match Interrupt for Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Enable_OCE1B:
       	push    temp1
	
       	in      temp1,TIMSK
       	sbr     temp1,(1<<OCIE1B)
       	out     TIMSK,temp1
	
       	pop     temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Enable_TICIE
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Enable Input Capture Interrupt for Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Enable_TICIE:
       	push    temp1
	
       	in      temp1,TIMSK
       	sbr     temp1,(1<<TICIE1)
       	out     TIMSK,temp1
	
       	pop     temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_Disable_TOIE
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disable Overflow Interrupt for TC0
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_Disable_TOIE:
       	push    temp1
	
       	in      temp1,TIMSK
       	cbr     temp1,(1<<TOIE0)
       	out     TIMSK,temp1
	
       	pop     temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_TOIE
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disable Overflow Interrupt for TC1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_TOIE:
       	push    temp1
	
       	in      temp1,TIMSK
       	cbr     temp1,(1<<TOIE1)
       	out     TIMSK,temp1
	
       	pop     temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_OCE1A 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disable Output CompareA Match Interrupt for Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_OCE1A:
	push	temp1
	
	in	temp1,TIMSK
	cbr	temp1,(1<<OCIE1A)
	out	TIMSK,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_OCE1B
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disable Output CompareB Match Interrupt for Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_OCE1B:
	push	temp1
	
	in	temp1,TIMSK
	cbr	temp1,(1<<OCIE1B)
	out	TIMSK,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_TICIE
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disable Input Capture Interrupt for Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_TICIE:
	push	temp1
	
	in	temp1,TIMSK
	cbr	temp1,(1<<TICIE1)
	out	TIMSK,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_Stop
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Stops the Timer/Counter0
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_Stop:
	push	temp1
	
	ldi	temp1,0x00
	out	TCCR0,temp1
	
	pop	temp1
	ret

 
;***********************************************************************
;* FUNCTION	: TIMER0_CK 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK for the prescaling source of Timer/Counter0
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_CK:
	push	temp1
	
	ldi	temp1,0x01
	out	TCCR0,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_CK_8 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/8 for the prescaling source of Timer/Counter0
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_CK_8:
	push	temp1
	
	ldi	temp1,0x02
	out	TCCR0,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_CK_64 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/64 for the prescaling source of Timer/Counter0
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_CK_64:
	push	temp1
	
	ldi	temp1,0x03
	out	TCCR0,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_CK_256 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/256 for the prescaling source of Timer/Counter0
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_CK_256:
	push	temp1
	
	ldi	temp1,0x04
	out	TCCR0,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_CK_1024 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/1024 for the prescaling source of Timer/Counter0
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_CK_1024:
	push	temp1
	
	ldi	temp1,0x05
	out	TCCR0,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_T0_HL 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define T0 with falling edge for the prescaling source of
;*		  Timer/Counter0
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_T0_HL:
	push	temp1
	
	ldi	temp1,0x06
	out	TCCR0,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_T0_LH 
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define T0 with rising edge for the prescaling source of
;*		  Timer/Counter0
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_T0_LH:
	push	temp1
	
	ldi	temp1,0x07
	out	TCCR0,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_Read_TCNT0
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the Timer Counter 0 Register
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : param1: Value of TCNT0
;*
;***********************************************************************
TIMER0_Read_TCNT0:
	in	param1,TCNT0
	ret

;***********************************************************************
;* FUNCTION	: TIMER0_Write_TCNT0
;*
;* CREATED	: 08.06.2000 by Jens Funke
;*
;* LAST CHANGED : 08.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Write param1 to TCNT0
;*
;* INPUT        : param1: Value to write to TCTN0
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_Write_TCNT0:
	out	TCNT0,param1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_OC1A 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disconnect Timer/Counter1 from output pin OC1A
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_OC1A:
	push	temp1
	
	in	temp1,TCCR1A
	cbr	temp1,(1<<COM1A1)
	cbr	temp1,(1<<COM1A0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Toggle_OC1A
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Toggle the OC1A output line
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Toggle_OC1A:
	push	temp1
	
	in	temp1,TCCR1A
	cbr	temp1,(1<<COM1A1)
	sbr	temp1,(1<<COM1A0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Clear_OC1A 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Clear the OC1A ouput line
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Clear_OC1A:
	push	temp1
	
	in	temp1,TCCR1A
	sbr	temp1,(1<<COM1A1)
	cbr	temp1,(1<<COM1A0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Set_OC1A
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Set the OC1A output line
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Set_OC1A:
	push	temp1
	
	in	temp1,TCCR1A
	sbr	temp1,(1<<COM1A1)
	sbr	temp1,(1<<COM1A0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_OC1B
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disconnect Timer/Counter1 from output pin OC1B
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_OC1B:
	push	temp1
	
	in	temp1,TCCR1A
	cbr	temp1,(1<<COM1B1)
	cbr	temp1,(1<<COM1B0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Toggle_OC1B 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Toggle the OC1B output line
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Toggle_OC1B:
	push	temp1
	
	in	temp1,TCCR1A
	cbr	temp1,(1<<COM1B1)
	sbr	temp1,(1<<COM1B0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Clear_OC1B 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Clear the OC1B ouput line
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Clear_OC1B:
	push	temp1
	
	in	temp1,TCCR1A
	sbr	temp1,(1<<COM1B1)
	cbr	temp1,(1<<COM1B0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Set_OC1B
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Set the OC1B output line
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Set_OC1B:
	push	temp1
	
	in	temp1,TCCR1A
	sbr	temp1,(1<<COM1B1)
	sbr	temp1,(1<<COM1B0)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_PWM 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disable PWM operation of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_PWM:
	push	temp1
	
	in	temp1,TCCR1A
	cbr	temp1,(1<<PWM11)
	cbr	temp1,(1<<PWM10)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_8Bit_PWM 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Timer/Counter is a 8-bit PWM
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_8Bit_PWM:
	push	temp1
	
	in	temp1,TCCR1A
	cbr	temp1,(1<<PWM11)
	sbr	temp1,(1<<PWM10)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_9Bit_PWM 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*                
;* DESCRIPTION  : Timer/Counter is a 9-bit PWM
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_9Bit_PWM:
	push	temp1
	
	in	temp1,TCCR1A
	sbr	temp1,(1<<PWM11)
	cbr	temp1,(1<<PWM10)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_10Bit_PWM 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Timer/Counter is a 10-bit PWM
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_10Bit_PWM:
	push	temp1
	
	in	temp1,TCCR1A
	sbr	temp1,(1<<PWM11)
	sbr	temp1,(1<<PWM10)
	out	TCCR1A,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Disable_NC_Capture 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Disable the noise canceler funktion of input capture
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Disable_NC_Capture:
	CBPORT	TCCR1B, ICNC1, temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Enable_NC_Capture 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Enable the niose canceler funktion of input capture
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Enable_NC_Capture:
	SBPORT	TCCR1B, ICNC1, temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_HL_Edge_Capture
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Transfer to Input Capture Reg on falling edge
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_HL_Edge_Capture:
	CBPORT	TCCR1B, ICNC1, temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_LH_Edge_Capture 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Transfer to Input Capture Reg on rising edge
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_LH_Edge_Capture:
	SBPORT	TCCR1B, ICNC1, temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Clear_on_Compare_Match
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Clear Timer/Counter1 on compare match
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Clear_on_Compare_Match:
	SBPORT	TCCR1B, CTC1, temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Continue_on_Compare_Match
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Timer/Counter1 continues counting on compare match
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Continue_on_Compare_Match:
	CBPORT	TCCR1B, CTC1, temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Stop
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Stops the Timer/Counter1
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Stop:
	push	temp1
	
	in	temp1,TCCR1B
	cbr	temp1,(1<<CS10)
	cbr	temp1,(1<<CS11)
	cbr	temp1,(1<<CS12)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

  endif	
;***********************************************************************
;* FUNCTION	: TIMER1_CK 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK for the prescaling source of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_CK:
	push	temp1
	
	in	temp1,TCCR1B
	cbr	temp1,(1<<CS12)
	cbr	temp1,(1<<CS11)
	sbr	temp1,(1<<CS10)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_CK_8 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/8 for the prescaling source of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_CK_8:
	push	temp1
	
	in	temp1,TCCR1B
	cbr	temp1,(1<<CS12)
	sbr	temp1,(1<<CS11)
	cbr	temp1,(1<<CS10)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_CK_64 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/64 for the prescaling source of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_CK_64:
	push	temp1
	
	in	temp1,TCCR1B
	cbr	temp1,(1<<CS12)
	sbr	temp1,(1<<CS11)
	sbr	temp1,(1<<CS10)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_CK_256
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/256 for the prescaling source of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_CK_256:
	push	temp1
	
	in	temp1,TCCR1B
	sbr	temp1,(1<<CS12)
	cbr	temp1,(1<<CS11)
	cbr	temp1,(1<<CS10)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_CK_1024
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define CK/1024 for the prescaling source of Timer/Counter1
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_CK_1024:
	push	temp1
	
	in	temp1,TCCR1B
	sbr	temp1,(1<<CS12)
	cbr	temp1,(1<<CS11)
	sbr	temp1,(1<<CS10)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

  ifdef all_code	
;***********************************************************************
;* FUNCTION	: TIMER1_T1_HL 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define T1 with falling edge for the prescaling source of
;*		  Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_T1_HL:
	push	temp1
	
	in	temp1,TCCR1B
	sbr	temp1,(1<<CS12)
	sbr	temp1,(1<<CS11)
	cbr	temp1,(1<<CS10)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_T1_LH
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Define T1 with rising edge for the prescaling source of
;*		  Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_T1_LH:
	push	temp1
	
	in	temp1,TCCR1B
	sbr	temp1,(1<<CS12)
	sbr	temp1,(1<<CS11)
	sbr	temp1,(1<<CS10)
	out	TCCR1B,temp1
	
	pop	temp1
	ret

  endif	
;***********************************************************************
;* FUNCTION	: TIMER1_Write_TCNT
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 14.02.2001 by Marc Dressler
;*
;* DESCRIPTION  : Write the TCNT1L and TCNT1H of Timer/Counter1
;*
;* INPUT        : param1 = value for TCNT1H (highbyte)
;*		  param2 = value for TCNT1L (lowbyte)
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Write_TCNT:
	push	tmph		; tmpl/h will be used by the controller
	push	tmpl		; to write the 16bit reg TCNT
	
	brid	TIMER_Write_TCNT_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to TCNT1
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	out	TCNT1H,param1	; ACHTUNG! Highbyte must be written first
	out	TCNT1L,param2	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	rjmp	TIMER_Write_TCNT_End
TIMER_Write_TCNT_NoINT:		; don't care about interrupts
	out	TCNT1H,param1	; ACHTUNG! Highbyte must be written first
	out	TCNT1L,param2	; high and lowbytes are written Simultaneously
TIMER_Write_TCNT_End:
	pop	tmpl
	pop	tmph
	ret
	
  ifdef all_code
;***********************************************************************
;* FUNCTION	: TIMER1_Read_TCNT
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the TCNT1L and TCNT1H of Timer/Counter1
;*
;* INPUT        : -
;*
;* OUTPUT       : param1 = value of TCNT1H (highbyte)
;*		  param2 = value of TCNT1L (lowbyte)
;*
;***********************************************************************
TIMER1_Read_TCNT:
	brid	TIMER_Read_TCNT_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to TCNT1
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	in	param2,TCNT1L	; ACHTUNG! Lowbyte must be written first
	in	param1,TCNT1H	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	ret
TIMER_Read_TCNT_NoINT:		; don't care about interrupts
	in	param2,TCNT1L	; ACHTUNG! Lowbyte must be written first
	in	param1,TCNT1H	; high and lowbytes are written Simultaneously
	ret

  endif		
;***********************************************************************
;* FUNCTION	: TIMER1_Write_OCR1A
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 14.02.2001 by Marc Dressler
;*
;* DESCRIPTION  : Write the OCR1AL and OCR1AH of Timer/Counter1
;*
;* INPUT        : param1 = value for OCR1AH (highbyte)
;*		  param2 = value for OCR1AL (lowbyte)
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Write_OCR1A:
	push	tmph		; tmpl/h will be used by the controller
	push	tmpl		; to write the 16bit reg OCR1A

	brid	TIMER_Write_OCR1A_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to OCR1A
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	out	OCR1AH,param1	; ACHTUNG! Highbyte must be written first
	out	OCR1AL,param2	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	rjmp	TIMER_Write_OCR1A_End
TIMER_Write_OCR1A_NoINT:	; don't care about interrupts
	out	OCR1AH,param1	; ACHTUNG! Highbyte must be written first
	out	OCR1AL,param2	; high and lowbytes are written Simultaneously

TIMER_Write_OCR1A_End:
	pop	tmpl
	pop	tmph
	ret

  ifdef all_code	
;***********************************************************************
;* FUNCTION	: TIMER1_Write_OCR1B
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 14.02.2001 by Marc Dressler
;*
;* DESCRIPTION  : Write the OCR1BL and OCR1BH of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : param1 = value for OCR1BH (highbyte)
;*		  param2 = value for OCR1BL (lowbyte)
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Write_OCR1B:
	push	tmph		; tmpl/h will be used by the controller
	push	tmpl		; to write the 16bit reg OCR1B

	brid	TIMER_Write_OCR1B_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to OCR1B
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	out	OCR1BH,param1	; ACHTUNG! Highbyte must be written first
	out	OCR1BL,param2	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	rjmp	TIMER_Write_OCR1B_End
TIMER_Write_OCR1B_NoINT:	; don't care about interrupts
	out	OCR1BH,param1	; ACHTUNG! Highbyte must be written first
	out	OCR1BL,param2	; high and lowbytes are written Simultaneously
TIMER_Write_OCR1B_End:
	pop	tmpl
	pop	tmph
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Read_OCR1A 
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the OCR1AL and OCR1AH of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : param1 = value of OCR1AH (highbyte)
;*		  param2 = value of OCR1AL (lowbyte)
;*
;***********************************************************************
TIMER1_Read_OCR1A:
	brid	TIMER_Read_OCR1A_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to OCR1A
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	in	param2,OCR1AL	; ACHTUNG! Lowbyte must be written first
	in	param1,OCR1AH	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	ret
TIMER_Read_OCR1A_NoINT:		; don't care about interrupts
	in	param2,OCR1AL	; ACHTUNG! Lowbyte must be written first
	in	param1,OCR1AH	; high and lowbytes are written Simultaneously
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Read_OCR1B
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the OCR1B1L and OCR1BH of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : param1 = value of OCR1BH (highbyte)
;*		  param2 = value of OCR1BL (lowbyte)
;*
;***********************************************************************
TIMER1_Read_OCR1B:
	brid	TIMER_Read_OCR1B_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to OCR1B
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	in	param2,OCR1BL	; ACHTUNG! Lowbyte must be written first
	in	param1,OCR1BH	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	ret
TIMER_Read_OCR1B_NoINT:		; don't care about interrupts
	in	param2,OCR1BL	; ACHTUNG! Lowbyte must be written first
	in	param1,OCR1BH	; high and lowbytes are written Simultaneously
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Write_ICR
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 14.02.2001 by Marc Dressler
;*
;* DESCRIPTION  : Write the ICR1L and ICR1H of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : param1 = value for ICR1H (highbyte)
;*		  param2 = value for ICR1L (lowbyte)
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Write_ICR:
	push	tmph		; tmpl/h will be used by the controller
	push	tmpl		; to write the 16bit reg ICR

	brid	TIMER_Write_ICR_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to ICR1
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	out	ICR1H,param1	; ACHTUNG! Highbyte must be written first
	out	ICR1L,param2	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	rjmp	TIMER_Write_ICR_End
TIMER_Write_ICR_NoINT:		; don't care about interrupts
	out	ICR1H,param1	; ACHTUNG! Highbyte must be written first
	out	ICR1L,param2	; high and lowbytes are written Simultaneously
TIMER_Write_ICR_End:
	pop	tmpl
	pop	tmph
	ret

;***********************************************************************
;* FUNCTION	: TIMER1_Read_ICR
;*
;* CREATED	: 20.06.2000 by Jens Funke
;*
;* LAST CHANGED : 20.06.2000 by Jens Funke
;*
;* DESCRIPTION  : Read the ICR1L and ICR1H of Timer/Counter1
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : param1 = value of ICR1H (highbyte)
;*		  param2 = value of ICR1L (lowbyte)
;*
;***********************************************************************
TIMER1_Read_ICR:
	brid	TIMER1_Read_ICR_NoINT	; if global interrupts are allowed
	; we have to disabled them else we can just write to ICR1
	cli			; disable the global interrupts because
	; the next Command is an 16bit access (2 cycles)
	in	param2,ICR1L	; ACHTUNG! Lowbyte must be written first
	in	param1,ICR1H	; high and lowbytes are written Simultaneously
	sei			; restore the old state
	ret
TIMER1_Read_ICR_NoINT:		; don't care about interrupts
	in	param2,ICR1L	; ACHTUNG! Lowbyte must be written first
	in	param1,ICR1H	; high and lowbytes are written Simultaneously
	ret



;***********************************************************************
;* FUNCTION	: TIMER1_Clear_TOV1
;*
;* CREATED	: 08/07/2000 by Jens Funke
;*
;* LAST CHANGED : 08/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Clears the TOV1-bit in TIFR. TOV1 is also cleared by hardware
;*		  when executing the corresponding interrupt
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Clear_TOV1:
	SBPORT	TIFR, TOV1, temp1
	ret

  endif
;***********************************************************************
;* FUNCTION	: TIMER1_Clear_OCF1A
;*
;* CREATED	: 08/07/2000 by Jens Funke
;*
;* LAST CHANGED : 08/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Clears the OCF1A-bit in TIFR. It is also cleared by hardware
;*		  when executing the corresponding interrupt
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Clear_OCF1A:
	SBPORT	TIFR, OCF1A, temp1
	ret

  ifdef all_code
;***********************************************************************
;* FUNCTION	: TIMER1_Clear_OCF1B
;*
;* CREATED	: 08/07/2000 by Jens Funke
;*
;* LAST CHANGED : 08/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Clears the OCF1B-bit in TIFR. It is also cleared by hardware
;*		  when executing the corresponding interrupt
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Clear_OCF1B:
	SBPORT	TIFR, OCF1B, temp1
	ret


;***********************************************************************
;* FUNCTION	: TIMER1_Clear_ICF1
;*
;* CREATED	: 08/07/2000 by Jens Funke
;*
;* LAST CHANGED : 08/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Clears the ICF1-bit in TIFR. It is also cleared by hardware
;*		  when executing the corresponding interrupt
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER1_Clear_ICF1:
	SBPORT	TIFR, ICF1, temp1
	ret


;***********************************************************************
;* FUNCTION	: TIMER0_Clear_TOV0 
;*
;* CREATED	: 08/07/2000 by Jens Funke
;*
;* LAST CHANGED : 08/07/2000 by Jens Funke
;*
;* DESCRIPTION  : Clears the TOV0-bit in TIFR. It is also cleared by hardware
;*		  when executing the corresponding interrupt
;*                (not tested yet)
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
TIMER0_Clear_TOV0:
	SBPORT	TIFR, TOV0, temp1
	ret

  endif


;***********************************************************************
		restore
	endif

;*
;* $Log: timer.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
