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
;* $Id: i2c.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : I2C Include-File
;*  Version         : 1.5
;*
;*  Filename        : i2c.inc
;*  Created         : 23.11.2000 by Marc Dressler
;*  Last changed    : 20.12.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : basic i2c functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : delay.inc
;*
;***********************************************************************

	ifndef	i2cinc	; prevent multiple binding

i2cinc	equ	1

		if	mompass=1
                 message "I2C Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "delay.inc"
	
;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

I2C_SCL		EQU	PB3	; AVR to I2C
I2C_SDAI	EQU	PB6	; I2C to AVR
I2C_SDAO	EQU	PB1	; AVR to I2C

I2C_SCL_PORT	EQU	PORTB
I2C_SDAI_PORT	EQU	PORTB
I2C_SDAO_PORT	EQU	PORTB

I2C_SCL_PIN	EQU	PINB
I2C_SDAI_PIN	EQU	PINB
I2C_SDAO_PIN	EQU	PINB

I2C_SCL_DDR	EQU	DDRB
I2C_SDAI_DDR	EQU	DDRB
I2C_SDAO_DDR	EQU	DDRB

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

        SEGMENT CODE

;***********************************************************************
;* FUNCTION	: I2C_Init
;*              
;* CREATED	: 13.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 05.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : Initialize I2C bus
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

I2C_Init:					
	sbi	I2C_SCL_DDR,I2C_SCL		;output
	sbi	I2C_SDAO_DDR,I2C_SDAO		;output

	cbi	I2C_SDAI_DDR,I2C_SDAI		;input
	sbi	I2C_SDAI_Port,I2C_SDAI		;

	rcall	I2C_Stop
	rcall	I2C_ReadBit
	rcall	I2C_Stop
	rcall	I2C_ReadBit
	rcall	I2C_Stop
	rcall	I2C_ReadBit
	rcall	I2C_Stop
	rcall	I2C_ReadBit
	rcall	I2C_Stop
	rcall	I2C_ReadBit
	rcall	I2C_Stop
	rcall	I2C_ReadBit
	rcall	I2C_Stop
	rcall	I2C_ReadBit
	rcall	I2C_Stop
	rcall	I2C_ReadBit

	rcall	I2C_Stop
	ret

;***********************************************************************
;* FUNCTION	: I2C_Start
;*              
;* CREATED	: 13.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 20.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : send start bit
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

I2C_Start:
  ifndef signals_inverted
	sbi	I2C_SCL_Port,I2C_SCL
	rcall	Delay5us

	sbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us

	cbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us

	cbi	I2C_SCL_Port,I2C_SCL
  endif
	
  ifdef signals_inverted
	cbi	I2C_SCL_Port,I2C_SCL
	rcall	Delay5us

	cbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us

	sbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us

	sbi	I2C_SCL_Port,I2C_SCL
  endif
	ret
		
;***********************************************************************
;* FUNCTION	: I2C_Stop
;*              
;* CREATED	: 13.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 20.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : send stop bit
;*
;* INPUT        : -
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

I2C_Stop:
  ifndef signals_inverted
	cbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us
	sbi	I2C_SCL_Port,I2C_SCL
 	rcall	Delay5us
	sbi	I2C_SDAO_Port,I2C_SDAO
  endif

  ifdef signals_inverted
	sbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us
	cbi	I2C_SCL_Port,I2C_SCL
 	rcall	Delay5us
	cbi	I2C_SDAO_Port,I2C_SDAO
  endif

	ret
		
;***********************************************************************
;* FUNCTION	: I2C_WriteBit
;*              
;* CREATED	: 13.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 20.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : send carry to i2c bus
;*
;* INPUT        : carry - bit
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

I2C_WriteBit:
	brcs	I2C_WB1
I2C_WB0:
  ifndef signals_inverted
	cbi	I2C_SDAO_Port,I2C_SDAO
  endif
  ifdef signals_inverted
	sbi	I2C_SDAO_Port,I2C_SDAO
  endif
	rjmp	I2C_WB
I2C_WB1:
  ifndef signals_inverted
	sbi	I2C_SDAO_Port,I2C_SDAO
  endif
  ifdef signals_inverted
	cbi	I2C_SDAO_Port,I2C_SDAO
  endif
I2C_WB:
	rcall	Delay5us
	
  ifndef signals_inverted
	sbi	I2C_SCL_Port,I2C_SCL
	rcall	Delay5us
	cbi	I2C_SCL_Port,I2C_SCL
  endif
  ifdef signals_inverted
	cbi	I2C_SCL_Port,I2C_SCL
	rcall	Delay5us
	sbi	I2C_SCL_Port,I2C_SCL
  endif
	ret

;***********************************************************************
;* FUNCTION	: I2C_ReadBit
;*              
;* CREATED	: 13.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 20.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : read bit from i2c bus to carry
;*
;* INPUT        : -
;*              
;* OUTPUT       : carry - bit 
;*
;***********************************************************************

I2C_ReadBit:
  ifndef signals_inverted
	sbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us
	sbi	I2C_SCL_Port,I2C_SCL
	rcall	Delay5us

	sec
	sbis	I2C_SDAI_Pin,I2C_SDAI
	clc
	
	cbi	I2C_SCL_Port,I2C_SCL
  endif
  ifdef signals_inverted
	cbi	I2C_SDAO_Port,I2C_SDAO
	rcall	Delay5us
	cbi	I2C_SCL_Port,I2C_SCL
	rcall	Delay5us

	sec
	sbic	I2C_SDAI_Pin,I2C_SDAI
	clc
	
	sbi	I2C_SCL_Port,I2C_SCL
  endif
	ret

;***********************************************************************
;* FUNCTION	: I2C_WriteByte
;*              
;* CREATED	: 13.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 28.08.2000 by Marc Dressler
;*              
;* DESCRIPTION  : send byte to i2c bus
;*
;* INPUT        : param1 - byte
;*              
;* OUTPUT       : - 
;*
;***********************************************************************

I2C_WriteByte:
	push	temp1

	ldi	temp1,8
I2C_WBY1:
	rol	param1
	rcall	I2C_WriteBit
	dec	temp1
	brne	I2C_WBY1
	
	rcall	I2C_ReadBit		;read ACK
;save carry
;before ret - reload carry 
	brcc	I2C_WBYE		;branch if ACK received (c=0) 
	rcall	I2C_Stop
I2C_WBYE:
	pop	temp1

	clc				;no error
	ret

;***********************************************************************
;* FUNCTION	: I2C_ReadByte
;*              
;* CREATED	: 13.11.1999 by Marc Dressler
;*              
;* LAST CHANGED : 20.12.2000 by Marc Dressler
;*              
;* DESCRIPTION  : send byte to i2c bus
;*		  set flag T to 1 to enable ACK
;*
;* INPUT        : T - bit (ACK on)
;*              
;* OUTPUT       : param1 - byte
;*
;***********************************************************************

I2C_ReadByte:
	push	temp1

	ldi	temp1,8
I2C_RBY1:
	rcall	I2C_ReadBit
	rol	param1
	dec	temp1
	brne	I2C_RBY1
	
;insert carry = T
;then delete sec

	sec				;set ACK = HIGH (no ACK from master)	
	rcall	I2C_WriteBit		;write ACK 
  ifndef signals_inverted
	sbi	I2C_SDAO_Port,I2C_SDAO
  endif
  ifdef signals_inverted
	cbi	I2C_SDAO_Port,I2C_SDAO
  endif
I2C_RBYE:
	pop	temp1

	clc				;no error
	ret

;***********************************************************************

	endif
	
;*
;* $Log: i2c.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
