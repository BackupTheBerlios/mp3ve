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
;* $Id: mp3veide.asm,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : MP3 IDE Main File
;*  Version         : 1.5
;*
;*  Filename        : mp3veide.asm
;*  Created         : 13.12.2000 by Marc Dressler
;*  Last changed    : 23.02.2002 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90S8515
;*  Target-PCB      : MP3veIDE V1.5
;*  Description     : main-file mp3ide 
;*
;*  Homepage        : www.MP3ve.de
;*		      www.uC-Elektronik.de
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
;*                    mp3.inc
;*                    dev.inc
;*                    rpc.inc
;*                    tocj.inc
;*
;***********************************************************************

;***********************************************************************
;* SWITCHES
;***********************************************************************

; debugging

;sys_debug		equ	1	; system debug informations
;  toc_debug		equ	1
;mp3_debug		equ	1	;
;mp3_retrigger_debug	equ	1	;
;sta_debug		equ	1	;
;atapi_slow_device	equ	1	; switch on if time to load the disc
					; is longer then 8 sec, e.g. for old devices
; system configuration

;stack_external		equ	1	; use xram as stack-memory
;MP3_NoQueue		equ	1	; without MP3-Queue
;MP3_NoReadErrorCheck	equ	1	; ignore read errors while playing
;cs4338			equ	1	; dac 16bit left padded (STA)
cs4331			equ	1	; dac 18bit left padded (MAS, STA)
;dac3550		equ		; dac (not implemented yet)
signals_inverted	equ	1	; mp3audio signals inverted (disable in Hardware Rev < 1.5)

atapi_int_enabled	equ	1	; switch off if access isn't possible
atapi_minspeed		equ	1	; speed down 

;rpc_disable		equ	1	; do not include rpc
rpc_old			equ	1	; use old rpc commands

STA_eeprom_protect	equ	1	; do not include functions for
					; EEPROM-programming
PlayList_disable	equ	1	; disable PlayList support
DevCopy_disabled	equ	1	; disable Dev_Copy
		
HW_Version		= ""
SW_Version		= ""
ID_Plain		= "MP3veH150S150(c)01/Dressler/Funke/Gutzeit/Steenbock"

;***********************************************************************

 if       mompass=1
   message "MP3veIDE V1.5 (C) 2002 by Marc Dressler, Jens Funke, Nils Steenbock, Thomas Gutzeit"
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
        INCLUDE "atapi.mac"
        INCLUDE "debug.mac"

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
        rjmp    Irq_INT0
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
        rjmp    Irq_SPI
        org     vec_uartrx      ;UART Receive Complete Interrupt Vector Address

  ifndef RPC_disable
        rjmp    RPC_Receive
  endif
  ifdef RPC_disable
        reti
  endif

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
; 0		= miniide-group(IDs 1-4) support@mp3ve.de
; 1		= Marc Dressler 	 marc@uc-elektronik.de
; 2		= Jens Funke		 jens@hardware-schmiede.de	
; 3		= Thomas Gutzeit	 gutzeit@upb.de
; 4		= Nils Steenbock	 nsteenb@upb.de
; 5-99 		= reserverd (ask miniide-group for ID)
; 100-255	= for free use

VersionPlain:
	data	ID_Plain, 0
VersionID:
	TWOBYTEDATA	1, 5,  1,  1, 1, 0	; Hardware Version (Vxx.yy) + Date (ddmmyy) + Producer-ID (z)
	TWOBYTEDATA	1, 5,  23,  3, 1, 0	; Software Version (Vxx.yy) + Date (ddmmyy) + Producer-ID (z)
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

	rcall	Debug_Init		; initialize build-in debug system

        rcall   UART_Init

  ifdef	sys_debug
					; send status message
	ldi	param1, RPC_CMD_SysReset
	sts	RPC_TxID, param1
	ldi	param1, 1
	sts	RPC_TxLen, param1 ; send 1 byte (RPC_TxData0)
	rcall	RPC_Transmit
  endif

  ifdef	sys_debug
	DEBUG_MSGID	DebugID_Sys_Reset
;	UART_PRINT "\nRESET"
  endif

	rcall	RPC_Init
	rcall	IRQ_RPCInit
	
        rcall   Error_Init

        rcall   I2C_Init
  
;        rcall   XRAM_Init

        rcall   SPI_Init

        rcall   STA_Init
  
        rcall   MP3_Init
        rcall   IRQ_MP3Init
	
  ifdef	sys_debug
	UART_PRINT "\nS:Dev_Init"
  endif
	rcall	DEV_Init		; init global variables and detect devices

  ifdef	sys_debug
	UART_PRINT "\nE:Dev_Init"
  endif
	
	rcall	DEV_Select_Master_CMD	; let's try the Master
	

  ifndef PlayList_disable
	rcall	PlayList_Init		; autostart check
  endif
	
  ifdef	sys_debug
  	DEBUG_MSGID	DebugID_Sys_Forever
;	UART_PRINT "\nS:Forever"
  endif

;***********************************************************************
;* Forever
;***********************************************************************

Forever:

; fill mp3 queue with data

  ifndef MP3_NoQueue
        rcall   MP3_Fetch			; fill queue
  endif
  
;       rcall   MP3_ServicePoll                 ; without IRQ-Support

; mp3 play after buffer underrun
	sbrc	MP3_Flags, MP3_funderrun
        rcall   MP3_PlayRetrigger

; rpc handling

  ifndef RPC_disable
	lds	temp1, RPC_Flags
	sbrc	temp1, RPC_fCmdReceived		; probe CMDRX-Flag 
        rcall   RPC_Command_Handler
  endif

  	rcall   Error_Service

        rjmp    Forever

;***********************************************************************
;* various functions
;***********************************************************************

;***********************************************************************
;* FUNCTION     : IRQ_MP3Init
;*
;* CREATED      : 13.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 13.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : enable MP3-IRQ
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

IRQ_MP3Init:
        push    temp1

        in      temp1, MCUCR
  ifndef signals_inverted
        ori     temp1, (1<<ISC01)+(1<<ISC00)	; MP3: INT0 on rising edge
  endif
  ifdef signals_inverted
        ori	temp1, (1<<ISC01)		; MP3: INT0 on falling edge
        andi	temp1, (0xff - (1<<ISC00))
  endif

        out     MCUCR, temp1

        ldi     temp1, (1<<INT0)                ; enable INT0 (MP3)
        out     GIMSK, temp1

        sbi     SPCR, SPIE                      ; enable SPI IRQ

        sei                                     ; enable irqs
        pop     temp1
        ret

;***********************************************************************
;* FUNCTION     : IRQ_RPCInit
;*
;* CREATED      : 13.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 13.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : enable RPC-IRQ
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

IRQ_RPCInit:
        sbi     UCR, RXCIE                      ; enable UART RX-Complete interrupt
        sei                                     ; enable irqs
        ret

;***********************************************************************
;* IRQ service functions
;***********************************************************************

Irq_INT0:
;       reti

        in      MP3_SREG, SREG
        rcall   MP3_Service
        out     SREG, MP3_SREG

        reti

Irq_SPI:
;       reti

        in      MP3_SREG, SREG
        rcall   MP3_ServiceSPI
        out     SREG, MP3_SREG

        reti

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

        INCLUDE "uart.inc"
        INCLUDE "xram.inc"
	INCLUDE "dev.inc"
        INCLUDE "mp3.inc"

  ifndef PlayList_disable
        INCLUDE "playlist.inc"
  endif
	
  ifndef RPC_disable
        INCLUDE "rpctab.inc"
        INCLUDE "tocj.inc"
  endif
  
  
        SEGMENT         DATA
DataSegEnd:                             ; end adress of internal used memory


;*
;* $Log: mp3veide.asm,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
