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
;* $Id: stapatch.asm,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : STA Patch Burner
;*  Version         : 1.5
;*
;*  Filename        : stapatch.asm
;*  Created         : 02.01.2001 by Nils Steenbock
;*  Last changed    : 23.02.2002 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90S8515
;*  Target-PCB      : MP3veIDE V1.5
;*  Description     : Bruning patch-file for STA-EEPROM
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : bitfuncs.inc
;*                    regavr.inc
;*                    8515def.inc
;*                    register.inc
;*                    macros.mac
;*                    uart.mac
;*                    uart.inc
;*                    xram.inc
;*
;***********************************************************************

;***********************************************************************
;* SWITCHES
;***********************************************************************

sta_debug		equ	1	;
signals_inverted	equ	1	; mp3audio signals inverted (disable in Hardware Rev < 1.5)



;***********************************************************************

 if       mompass=1
   message "STAPATCH V1.5 (C) 2002 by Marc Dressler, Jens Funke, Nils Steenbock, Thomas Gutzeit"
 endif

;***********************************************************************
;* GLOBAL DEFINITIONS
;***********************************************************************

        CPU AT90S8515
        RELAXED ON      ;Constants in C-Syntax
        WRAPMODE ON

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

        INCLUDE "bitfuncs.inc"
        INCLUDE "regavr.inc"
        INCLUDE "8515def.inc"
        INCLUDE "register.inc"
        INCLUDE "macros.mac"
        INCLUDE "uart.mac"

;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

  ifdef stack_external
STACKEND	= 0x7fff
  endif
  
  ifndef stack_external
STACKEND	= IRAMEND  
  endif
  

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

        SEGMENT DATA

;* INTERNAL MEMORY
        org     IRAM            ; jump behind internal registers

;* EXTERNAL MEMORY
  ifndef stack_external
        org     IRAMEND+1	; jump to external memory
  endif

;***********************************************************************
;* IRQ vectors
;***********************************************************************

        SEGMENT CODE

        org     0x00            ;reset handle
        rjmp    Reset
        org     vec_int0        ;External Interrupt0 Vector Address
	reti
        org     vec_int1        ;External Interrupt1 Vector Address
        reti     
        org     vec_tm1capt     ;Input Capture1 Interrupt Vector Address
        reti
        org     vec_tm1compa    ;Output Compare1A Interrupt Vector Address
        reti
        org     vec_tm1compb    ;Output Compare1B Interrupt Vector Address
        reti
        org     vec_tm1ovf      ;Overflow1 Interrupt Vector Address
        reti
        org     vec_tm0ovf      ;Overflow0 Interrupt Vector Address
        reti
        org     vec_spi         ;SPI Interrupt Vector Address
	reti
        org     vec_uartrx      ;UART Receive Complete Interrupt Vector Address
	reti
        org     vec_uartudre    ;UART Data Register Empty Interrupt Vector Address
        reti
        org     vec_uarttx      ;UART Transmit Complete Interrupt Vector Address
        reti
        org     vec_anacomp     ;Analog Comparator Interrupt Vector Address
        reti

;***********************************************************************
;* Version IDs
;***********************************************************************

; Producer-IDs
; 0		= miniide-group(IDs 1-4) miniide@upb.de
; 1		= Marc Dressler 	 marc@uc-elektronik.de
; 2		= Jens Funke		 jens@hardware-schmiede.de	
; 3		= Thomas Gutzeit	 gutzeit@upb.de
; 4		= Nils Steenbock	 nsteenb@upb.de
; 5-199 	= reserverd (ask miniide-group for ID)
; 200-255	= for free use

VersionPlain:
	data	"MP3IDE HW V01.50 01.01.2001 SW V01.50 14.01.2001 (c) M.Dressler, J.Funke, T.Gutzeit, N.Steenbock",0
VersionID:
	data	1, 5,  1,  1, 1	; Hardware Version + Date
	data	0		; Producer-ID (0 = miniide-group)
	data	1, 5, 14,  1, 1	; Software Version + Date
	data	0		; Producer-ID (0 = miniide-group)
VersionEnd:

;***********************************************************************
;* MAIN function
;***********************************************************************

Main:
Reset:
	in      temp1, MCUCR
	sbr     temp1, (1<<SRE)		; XRAM on
;	sbr     temp1, (1<<SRW)		; waitstate on
	out     MCUCR, temp1		; 

        ldi     temp1, lo(STACKEND)
        out     SPL, temp1              ; init stackpointer
        ldi     temp1, hi(STACKEND)
        out     SPH, temp1

        rcall   UART_Init

  ifdef	sys_debug
	UART_PRINT "\nSystem_Reset"
  endif

        rcall   I2C_Init
  
  
	
;***********************************************************************
;* MAIN
;***********************************************************************

	UART_Print "Burning Patch-EEPROM\n\r"
	rcall STA_BurnPatch
	UART_Print "Done.\n\r"
	
Forever:
	rjmp	Forever


;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

        INCLUDE "uart.inc"
        INCLUDE "xram.inc"

	INCLUDE "stapatch.inc"
	
        SEGMENT         DATA
DataSegEnd:                             ; end adress of internal used memory

;*
;* $Log: stapatch.asm,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
