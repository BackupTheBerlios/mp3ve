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
;* $Id: mp3.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : MP3 Include-File
;*  Version         : 1.5
;*
;*  Filename        : mp3.inc
;*  Created         : 26.04.2000 by Marc Dressler
;*  Last changed    : 01.04.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : basic mp3 functions
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : spi.inc
;*		      mas.inc
;*		      sta.inc
;*		      dev.inc
;*		      mp3ser.inc
;*
;***********************************************************************

	ifndef	mp3inc	; prevent multiple binding

mp3inc	equ	1

		if	mompass=1
                 message "MP3 Include-File V1.5 (C)2000 Marc Dressler"
                endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

	INCLUDE "mp3ser.inc"
	INCLUDE "mas.inc"
	INCLUDE "sta.inc"
	INCLUDE "error.inc"
	INCLUDE	"spi.inc"
	INCLUDE	"dev.inc"
	
;***********************************************************************
;* USER DEFINITIONS
;***********************************************************************

MP3_DAREQ	EQU	PD2
MP3_DAREQ_PORT	EQU	PORTD
MP3_DAREQ_PIN	EQU	PIND
MP3_DAREQ_DDR	EQU	DDRD

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

	SEGMENT DATA

;MP3_QSize	equ	400
;MP3_QStart	equ	0x0260
;MP3_QEnd	equ	MP3_QStart+MP3_QSize+1

MP3_QStart	equ	DataSegEnd

;  ifndef stack_external
;MP3_QEnd	equ	0x8000
;  endif

;  ifdef stack_external
MP3_QEnd	equ	0x8000-1024
;  endif

					; MP3_Flags Register: [default]
					; status flags for mp3-chip support
MP3_Fenabled	equ	0		; bit 0 [0] - mp3 enabled(1)
MP3_Fspirunning	equ	1		; bit 1 [0] - spi running(1)
MP3_Fempty	equ	2		; bit 2 [1] - queue empty(1)
MP3_Ffull	equ	3		; bit 3 [0] - queue full(1)
MP3_Funderrun	equ	4		; bit 4 [0] - queue underrun(1)
MP3_Ftmpdata	equ	5		; bit 5 [0] - tempdata full (1)
MP3_Ffileend	equ	6		; bit 6 [0] - end of file reached (1)
MP3_Finvalide	equ	7		; bit 7 [0] - read data invalide (1)

					; MP3_Flags1 Register: [default]
					; status flags for mp3-chip support
MP3_Fsoftstop	equ	0		; bit 0 [0] - stop after queue empty(1)
MP3_Fautoskip	equ	1		; bit 1 [0] - endless play of song = auto skip (1)
MP3_F2	equ	2			; bit 2 [0] - 
MP3_F3	equ	3			; bit 3 [0] - 
MP3_F4	equ	4			; bit 4 [0] - 
MP3_F5	equ	5			; bit 5 [0] - 
MP3_F6	equ	6			; bit 6 [0] - 
MP3_F7	equ	7			; bit 7 [0] - 


; parameter of active song

MP3_Sector:
MP3_Sector1:		res	1	; LBA (LSB)
MP3_Sector2:		res	1
MP3_Sector3:		res	1
MP3_Sector4:		res	1	; LBA (MSB)

MP3_SectorCount:
MP3_SectorCount1:	res	1	; (LSB)
MP3_SectorCount2:	res	1
MP3_SectorCount3:	res	1
MP3_SectorCount4:	res	1	; (MSB)

MP3_Source:		res	1	
					; MP3_Source: [default]
					; 
MP3_Source_F0		equ	0	; bit 0 [0] - media LSB
MP3_Source_F1		equ	1	; bit 1 [0] - media ...
MP3_Source_F2		equ	2	; bit 2 [0] - media ...
MP3_Source_F3		equ	3	; bit 3 [0] - media MSB
MP3_Source_Fslave	equ	4	; bit 4 [0] - master (0) / slave (1)
MP3_Source_Flpt		equ	5	; bit 5 [0] - ide (0) / other(lpt) (1)
MP3_Source_Faudiotrack	equ	6	; bit 6 [0] - mp3file (0) / audiotrack (1)
MP3_Source_Fdirectory	equ	7	; bit 7 [0] - [file (0) / directory (1)] 

; parameter for next song

MP3_NextSector:
MP3_NextSector1:	res	1	; LBA (LSB)
MP3_NextSector2:	res	1
MP3_NextSector3:	res	1
MP3_NextSector4:	res	1	; LBA (MSB)

MP3_NextSectorCount:
MP3_NextSectorCount1:	res	1	; (LSB)
MP3_NextSectorCount2:	res	1
MP3_NextSectorCount3:	res	1
MP3_NextSectorCount4:	res	1	; (MSB)

MP3_NextSource:		res	1

MP3_Chip:		res	1	; detected Chip   XXX not full implemented yet XXX
					; 0 - none/unknown
					; 1 - mas 3507D
					; 2 - sta 013
					; 3 - sta 015
					; x - reserved

MP3_ErrorFlags:		res	1	; errors while playing 

        SEGMENT CODE


;***********************************************************************
;* FUNCTION	: MP3_Init
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 14.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : init mp3 subsystem
;*                call only once
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Init:
  ifdef	mp3_debug
	UART_PRINT "\nS:3_Init"
  endif
	push	temp1

	cbi	MP3_DAREQ_DDR,MP3_DAREQ		; input
	sbi	MP3_DAREQ_PORT,MP3_DAREQ	; pullup on

	ldi	temp1, 0x00
	sts	MP3_ErrorFlags, temp1

	ldi	temp1, 0
	sts	MP3_Chip, temp1			; chip unknown

  	ldi	MP3_Flags, (1<<MP3_fempty)	; default settings

	ldi	temp1, 0x00
  	mov	MP3_Flags1, temp1		; default settings

	pop	temp1
  
	rcall	MP3_ReInit
  ifdef	mp3_debug
	UART_PRINT "\nE:3_Init"
  endif
	ret

;***********************************************************************
;* FUNCTION	: MP3_ReInit
;*
;* CREATED	: 19.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 31.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : init queue after using queue for other purposes
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_ReInit:
  ifdef	mp3_debug
	UART_PRINT "\nS:3_ReInit"
  endif
	push	temp1
  	ldi	MP3_Flags, (1<<MP3_fempty)	; default settings

	ldi	temp1, lo(MP3_QStart)	; init queue pointers
	mov	MP3_TailLo, temp1
	mov	MP3_HeadLo, temp1

	ldi	temp1, hi(MP3_QStart)
	mov	MP3_TailHi, temp1
	mov	MP3_HeadHi, temp1

	pop	temp1
  ifdef	mp3_debug
	UART_PRINT "\nE:3_ReInit"
  endif
	ret

;***********************************************************************
;* FUNCTION	: MP3_Flush
;*
;* CREATED	: 16.07.2000 by Marc Dressler
;*
;* LAST CHANGED : 27.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : clear mp3-queue
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Flush:
  ifdef	mp3_debug
	UART_PRINT "\nS:3_Flush"
  endif
	rcall	MP3_ReInit
  ifdef	mp3_debug
	UART_PRINT "\nS:3_DFlush"
  endif
	rcall	DEV_ClearBuffer		; clear DEV-Buffer
  ifdef	mp3_debug
	UART_PRINT "\nE:3_DFlush"
	UART_PRINT "\nE:3_Flush"
  endif
	ret

;***********************************************************************
;* FUNCTION	: MP3_Enqueue
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 15.09.2000 by Marc Dressler
;*
;* DESCRIPTION  : insert one byte in mp3-queue
;*
;* INPUT        : param1 - data byte to enqueue
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Enqueue:
	sbrc	MP3_Flags, MP3_Ffull	; check if queue full
	rjmp	MP3_EnOverrun		; overrun error  !!

	push	xh
	push	xl

	mov	xh, MP3_TailHi
	mov	xl, MP3_TailLo
;	movw	xl, MP3_TailLo

	st	X+, param1		; store data byte in queue

	cpi	xl, lo(MP3_QEnd)	; end of queue reached ?
	brne	MP3_EnNEnd
	cpi	xh, hi(MP3_QEnd)
	brne	MP3_EnNEnd
	ldi	xl, lo(MP3_QStart)	; init queue tail pointer
	ldi	xh, hi(MP3_QStart)
MP3_EnNEnd:
	cli				; critical section - disable irqs
	  
	mov	MP3_TailHi, xh
	mov	MP3_TailLo, xl
;	movw	MP3_TailLo, xl

	cbr	MP3_Flags, (1<<MP3_Fempty); queue not empty

	cpse	MP3_HeadLo, MP3_TailLo	; queue full if Head = Tail
	rjmp	MP3_EnNotFull
	cpse	MP3_HeadHi, MP3_TailHi
	rjmp	MP3_EnNotFull
	sbr	MP3_Flags, (1<<MP3_Ffull)	; set flag queue full
MP3_EnNotFull:
;	sbrc	MP3_Flags, MP3_Fenabled	; skip if mp3 not enabled
;	rcall	MP3_Service		; initial call

	sei				; end of critical section - enable irqs
	 
	pop	xl
	pop	xh

	ret
MP3_EnOverrun:
	;; INSERT ERROR MESSAGE HERE
	;; ERROR IN ASSEMBLER CODE IF OCCURS !!

	ret

;***********************************************************************
;* FUNCTION	: MP3_Dequeue
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 18.09.2000 by Marc Dressler
;*
;* DESCRIPTION  : remove one byte from mp3-queue
;*
;* INPUT        : -
;*
;* OUTPUT       : param1 - byte dequeued
;*
;***********************************************************************

MP3_Dequeue:
	sbrc	MP3_Flags, MP3_Fempty	; check if queue empty
	rjmp	MP3_DeUnderrun		; underrun error

	ld	param1, Y+		; get data from queue

	cpi	MP3_HeadLo, lo(MP3_QEnd)	; end of queue reached ?
	brne	MP3_DeNEnd
	cpi	MP3_HeadHi, hi(MP3_QEnd)
	brne	MP3_DeNEnd

	ldi	MP3_HeadLo, lo(MP3_QStart)	; init queue head pointer
	ldi	MP3_HeadHi, hi(MP3_QStart)
MP3_DeNEnd:
	cbr	MP3_Flags, (1<<MP3_Ffull); queue not full

	cpse	MP3_HeadLo, MP3_TailLo	; queue empty if Head = Tail
	rjmp	MP3_DeNotEmpty
	cpse	MP3_HeadHi, MP3_TailHi
	rjmp	MP3_DeNotEmpty
	sbr	MP3_Flags, (1<<MP3_Fempty)	; set flag queue empty
MP3_DeNotEmpty:
	ret
MP3_DeUnderrun:
	;; INSERT ERROR MESSAGE HERE
	;; ERROR IN ASSEMBLER CODE IF OCCURS !!

	ret

;***********************************************************************
;* FUNCTION	: MP3_Play
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 19.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : start playing
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Play:
  ifdef	mp3_debug
	UART_PRINT "\nS:3_Play"
  endif

	rcall	MP3_NoSoftStop
	
	cbr	MP3_Flags, (1<<MP3_funderrun)
	sbr	MP3_Flags, (1<<MP3_fenabled)

	rcall	MP3_Service			; initial call
	sei

  ifdef	mp3_debug
	UART_PRINT "\nE:3_Play"
  endif
	ret

;***********************************************************************
;* FUNCTION	: MP3_Pause
;*
;* CREATED	: 16.08.2000 by Marc Dressler
;*
;* LAST CHANGED : 16.08.2000 by Marc Dressler
;*
;* DESCRIPTION  : pause playing
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Pause:
  ifdef	mp3_debug
	UART_PRINT "\nSE:3_Pause"
  endif
	cbr	MP3_Flags, (1<<MP3_fenabled)
	ret

;***********************************************************************
;* FUNCTION	: MP3_Stop
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 27.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : stop playing immediately
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Stop:
  ifdef	mp3_debug
	UART_PRINT "\nS:3_Stop"
  endif

	cbr	MP3_Flags, (1<<MP3_fenabled)
	rcall	MP3_Flush

  ifdef	mp3_debug
	UART_PRINT "\nE:3_Stop"
  endif
;	ret

;***********************************************************************
;* FUNCTION	: MP3_SoftStop
;*
;* CREATED	: 17.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 17.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : stop playing after queue empty
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_SoftStop:
  ifdef	mp3_debug
	UART_PRINT "\nS:3_SStop"
  endif
	push	temp1
	mov	temp1, MP3_Flags1 
	sbr	temp1, (1<<MP3_fsoftstop)
	mov	MP3_Flags1, temp1
	pop	temp1
  ifdef	mp3_debug
	UART_PRINT "\nE:3_SStop"
  endif
	ret

;***********************************************************************
;* FUNCTION	: MP3_NoSoftStop
;*
;* CREATED	: 19.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 19.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : no soft stop
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_NoSoftStop:
  ifdef	mp3_debug
	UART_PRINT "\nS:3_NSStop"
  endif
	push	temp1
	mov	temp1, MP3_Flags1 
	cbr	temp1, (1<<MP3_fsoftstop)
	mov	MP3_Flags1, temp1
	pop	temp1
  ifdef	mp3_debug
	UART_PRINT "\nE:3_NSStop"
  endif
	ret

;***********************************************************************
;* FUNCTION	: MP3_AutoSkipOn
;*
;* CREATED	: 17.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 17.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : endless play of track 
;*	          or play next track automatically
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_AutoSkipOn:
	push	temp1
	mov	temp1, MP3_Flags1
	sbr	temp1, (1<<MP3_fautoskip)
	mov	MP3_Flags1, temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: MP3_AutoSkipOff
;*
;* CREATED	: 17.12.2000 by Marc Dressler
;*
;* LAST CHANGED : 17.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : play track only once
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_AutoSkipOff:
	push	temp1
	mov	temp1, MP3_Flags1
	cbr	temp1, (1<<MP3_fautoskip)
	mov	MP3_Flags1, temp1
	pop	temp1
	ret

;***********************************************************************
;* FUNCTION	: MP3_Skip
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 07.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : play next song
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************
MP3_Skip:					; with Flush
  ifdef	mp3_debug
	UART_PRINT "\nS:3_Skip"
  endif

	rcall	MP3_Flush

	push	temp1
	rjmp	MP3_Skip1

MP3_AutoSkip:					; without Flush
  ifdef	mp3_debug
	UART_PRINT "\nS:3_ASkip"
  endif
	push	temp1

	sbrs	MP3_Flags1, MP3_fautoskip	; play next track automatically ? 
	rjmp	MP3_NoAutoSkip			; no
;yes
	sbr	MP3_Flags, (1<<MP3_Ffileend)	; file-end reached after 2 sec at 128Kbit

MP3_Skip1:
	lds	temp1, MP3_NextSector1
	sts	MP3_Sector1, temp1
	lds	temp1, MP3_NextSector2
	sts	MP3_Sector2, temp1
	lds	temp1, MP3_NextSector3
	sts	MP3_Sector3, temp1
	lds	temp1, MP3_NextSector4
	sts	MP3_Sector4, temp1

	lds	temp1, MP3_NextSectorCount1
	sts	MP3_SectorCount1, temp1
	lds	temp1, MP3_NextSectorCount2
	sts	MP3_SectorCount2, temp1
	lds	temp1, MP3_NextSectorCount3
	sts	MP3_SectorCount3, temp1
	lds	temp1, MP3_NextSectorCount4
	sts	MP3_SectorCount4, temp1

	lds	temp1, MP3_NextSource
	sts	MP3_Source, temp1

	; no new job to play ?
	lds	temp1, MP3_SectorCount1
	tst	temp1
	brne	MP3_Skip_End		; <> 0
	lds	temp1, MP3_SectorCount2
	tst	temp1
	brne	MP3_Skip_End		; <> 0
	lds	temp1, MP3_SectorCount3
	tst	temp1
	brne	MP3_Skip_End		; <> 0
	lds	temp1, MP3_SectorCount4
	tst	temp1
	brne	MP3_Skip_End		; <> 0

MP3_NoAutoSkip:
MP3_Skip_NoJob:

	rcall	MP3_SoftStop

	sbr	MP3_Flags, (1<<MP3_Finvalide)

	sbrc	MP3_Flags, MP3_Fempty	; skip if queue not empty
	sbr	MP3_Flags, (1<<MP3_Ffileend)	; file-end reached
	sbrc	MP3_Flags, MP3_Fempty	; skip if queue not empty
	cbr	MP3_Flags, (1<<MP3_fenabled)	; mp3_stop without BufferClear
 
MP3_Skip_End:
	pop	temp1
  ifdef	mp3_debug
	UART_PRINT "\nE:3_Skip"
  endif
	ret

;***********************************************************************
;* FUNCTION	: MP3_Service
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 21.06.2000 by Marc Dressler
;*
;* DESCRIPTION  : data request irq task
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Service:
	sbrc	MP3_Flags, MP3_Fspirunning	; skip if spi running
	ret

	sbi	SPCR, SPIE			; enable SPI IRQ
;	rjmp	MP3_ServiceSPI			; not running

;***********************************************************************
;* FUNCTION	: MP3_ServiceSPI
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 31.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : spi-irq task
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_ServiceSPI:
	push	param1
	in	param1, SPSR		; 
	pop	param1

  ifndef signals_inverted
	sbis	MP3_DAREQ_Pin, MP3_DAREQ; high on data requested
  endif
  
  ifdef signals_inverted
	sbic	MP3_DAREQ_Pin, MP3_DAREQ; low on data requested
  endif

	rjmp	MP3_SpiRhi

  ifdef rpc_old
	sbrs	MP3_Flags, MP3_fenabled	; skip if playing enabled
	rjmp	MP3_SpiRhi		; disabled
  endif

  ifndef MP3_NoQueue
	sbrc	MP3_Flags, MP3_Fempty	; skip if queue not empty
	rjmp	MP3_SpiEmpty		; queue is empty
  endif

  ifndef rpc_old
	sbrs	MP3_Flags, MP3_fenabled	; skip if playing enabled
	rjmp	MP3_SpiRhi		; disabled
  endif

MP3_SpiRlo:				; start spi
	sbr	MP3_Flags, (1<<MP3_Fspirunning)

	push	param1

	in	param1, GIMSK 
	cbr	param1, (1<<INT0)	; disable INT0 (MP3)
	out	GIMSK, param1
	
  ifndef MP3_NoQueue
	rcall	MP3_Dequeue
  endif
  
  ifdef MP3_NoQueue
	rcall	MP3_ReadDevice
  endif

; SPI_Write
;	sbi	SPCR, MSTR
;	sbi	SPCR, SPE			;enable SPI

  ifdef signals_inverted
  	com	param1
  endif
	out	SPDR, param1

	pop	param1
	ret

MP3_SpiEmpty:				; queue underrun
	sbrs	MP3_Flags1, MP3_fsoftstop
	rjmp	MP3_SpiEmpty1

	sbr	MP3_Flags, (1<<MP3_Ffileend); track-end reached
  
	cbr	MP3_Flags, (1<<MP3_fenabled)	; = MP3_Stop without Flush
	rcall	MP3_NoSoftStop
  
	rjmp	MP3_SpiRhi
MP3_SpiEmpty1:
	sbr	MP3_Flags, (1<<MP3_Funderrun); set flag underrun

MP3_SpiRhi:				; stop spi
	cbr	MP3_Flags, (1<<MP3_Fspirunning)
	cbi	SPCR, SPIE		; disable SPI IRQ

	push	param1	
	in	param1, GIMSK 
	sbr	param1, (1<<INT0)	; enable INT0 (MP3)
	out	GIMSK, param1
	pop	param1	

;	rcall	SPI_Stop
	ret

;***********************************************************************
;* FUNCTION	: MP3_ServicePoll
;*
;* CREATED	: 21.06.2000 by Marc Dressler
;*
;* LAST CHANGED : 21.06.2000 by Marc Dressler
;*
;* DESCRIPTION  : MP3_Service without IRQ
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_ServicePoll:
	sbic	SPSR,SPIF		; wait until transfer complete
	rjmp	MP3_ServiceSPI
	ret

;***********************************************************************
;* FUNCTION	: MP3_PlayRetrigger
;*
;* CREATED	: 09.07.2000 by Marc Dressler
;*
;* LAST CHANGED : 31.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : start play in case of buffer underrun
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_PlayRetrigger:
	sbrs	MP3_Flags, MP3_funderrun
	ret
;	sbrc	MP3_Flags1, MP3_fsoftstop
;	ret
	sbrs	MP3_Flags, MP3_fenabled
	ret
	sbrs	MP3_Flags, MP3_Ffull		; only retrigger if queue full
	ret
;	sbrc	MP3_Flags, MP3_Fempty		; only retrigger if queue not empty
;	ret

  ifdef	mp3_retrigger_debug
	UART_PRINT "\n3_R"
  endif

	cbr	MP3_Flags, (1<<MP3_Funderrun)	; reset flag

	sbi	SPCR, SPIE			; enable SPI IRQ
	
	rjmp	MP3_ServiceSPI			; restart playing
	;ret					; not nessesary

;***********************************************************************
;* FUNCTION	: MP3_Fetch
;*
;* CREATED	: 09.07.2000 by Marc Dressler
;*
;* LAST CHANGED : 02.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : fill mp3-cache with data from hdd or cd
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_Fetch:
	sbrc	MP3_Flags, MP3_Ffull	; skip if MP3 queue not full
	ret				; is full -> end
	sbrs	MP3_Flags, MP3_fenabled
	ret
	
; other sources - include here !!

MP3_FetchIDE:
	push	param1
	rcall	MP3_ReadDevice		; get character from selected device

	sbrs	MP3_Flags, MP3_Finvalide; if sucess  
	rcall	MP3_Enqueue		; put into MP3-Queue

	pop	param1
	ret	

;***********************************************************************
;* FUNCTION	: MP3_SetVolume
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 20.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : set volume
;*
;* INPUT        : param2 - volume
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_SetVolume:
	rcall	MAS_SetVolume
	rcall	STA_SetVolume
	ret

;***********************************************************************
;* FUNCTION	: MP3_SetSpeed
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 17.12.2000 by Marc Dressler
;*
;* DESCRIPTION  : set play-speed
;*		  ! not implementet yet !
;*
;* INPUT        : param1 - speed
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_SetSpeed:
	ret

;***********************************************************************
;* FUNCTION	: MP3_GetCurrentTrack
;*
;* CREATED	: 09.05.2000 by Marc Dressler
;*
;* LAST CHANGED : 19.01.2001 by Marc Dressler
;*
;* DESCRIPTION  : send current position to uart
;*
;* INPUT        : -
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_GetCurrentTrack:
	push	param1

	ldi	param1, RPC_CMD_Get_CurrentTrack
	sts	RPC_TxID, param1	; ID 
	
	lds	param1, MP3_Sector4	; (msb)
	sts	RPC_TxData0, param1
	lds	param1, MP3_Sector3
	sts	RPC_TxData1, param1	
	lds	param1, MP3_Sector2
	sts	RPC_TxData2, param1	
	lds	param1, MP3_Sector1	; (lsb)
	sts	RPC_TxData3, param1

	lds	param1, MP3_SectorCount4; (msb)
	sts	RPC_TxData4, param1
	lds	param1, MP3_SectorCount3
	sts	RPC_TxData5, param1
	lds	param1, MP3_SectorCount2
	sts	RPC_TxData6, param1
	lds	param1, MP3_SectorCount1; (lsb)
	sts	RPC_TxData7, param1

	lds	param1, MP3_Source
	sts	RPC_TxData8, param1

	ldi	param1, 9
	sts	RPC_TxLen, param1
	pop	param1

	rcall	RPC_Transmit
	ret


;***********************************************************************
;* FUNCTION	: MP3_Select_Source
;*
;* CREATED	: 20.01.2001 by Marc Dressler
;*
;* LAST CHANGED : 01.04.2001 by Marc Dressler
;*
;* DESCRIPTION  : select audio source
;* 	          audio, mp3, master, slave, slot, ...
;*
;* INPUT        : param1 - source
;*
;* OUTPUT       : -
;*
;***********************************************************************

MP3_SelectSource:
	rcall	MP3_Stop
	sts	MP3_Source, param1
	sbrs	param1, MP3_Source_Fslave
	rjmp	DEV_Select_Master_CMD
	rjmp	DEV_Select_Slave_CMD
;	ret

;***********************************************************************

	endif

;*
;* $Log: mp3.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
