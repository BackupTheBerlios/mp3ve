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
;* $Id: tocj.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : Read-Joliet-TOC
;*  Version         : 1.5
;*
;*  Filename        : tocj.inc
;*  Created         : 16.05.2000 by Nils Steenbock
;*  Last changed    : 06.01.2001 by Nils Steenbock
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V1.5
;*  Description     : TOC-reading routine 
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : bitfuncs.inc
;*		      regavr.inc
;*		      register.inc	
;*		      uart.inc  
;*		      dev.inc
;*                    atapi.inc
;*		      ata.inc
;*
;***********************************************************************

 	ifndef	tocjinc		;prevent multiple binding

tocjinc	equ	1

	if       mompass=1
	  message "ReadTOC V1.5 (C) 2000 by Nils Steenbock"
	endif

;***********************************************************************
;* INCLUDE FILES
;***********************************************************************

        INCLUDE "bitfuncs.inc"		; includes functions like lo() or hi()
        INCLUDE "regavr.inc"	   
        INCLUDE "register.inc"

;***********************************************************************
;* PRIVATE DEFINITIONS
;***********************************************************************

TOC_Data	 EQU	0x2400	        ; temporary sector data
TOC_SubDirStart	 EQU	0x6400		; array of sub directory start sectors
TOC_SubDirLength EQU	0x6420		; length of subdir in sectors
TOC_SubDirCursor EQU	0x6428		; cursor for different dir levels
TOC_Current	 EQU	0x6438		; temporary storage of cur. entry
TOC_Flags	 EQU	0x6638		; RAM Address for some flags
	
        SEGMENT CODE



;***********************************************************************
;* FUNCTION	: TOC_Read
;*              
;* CREATED	: 16.05.2000 by Nils Steenbock
;*              
;* LAST CHANGED : 06.01.2000 by Nils Steenbock
;*              
;* DESCRIPTION  : read TOC-data from the selected device
;*
;* INPUT        : -
;*              
;* OUTPUT       : -
;*              
;***********************************************************************
	
TOC_Read:				   
  ifdef	toc_debug
	UART_PRINT "\nS:T_Read"
  endif
	cli	

	push	XL
	push	XH
	push	YL
	push	YH
	push	ZL
	push	ZH
	push	temp1			; save temp values
	push	temp2
	push	temp3
	push	temp4
	push	temp5
	push	temp6
	push	tmpL
	push 	tmpH
	push	param4
	push 	param3
	push 	param2
	push 	param1

	ldi     tmpH, 3

TOC_InitMedia:

		ifdef	toc_debug
	UART_PRINT "\n\rInit_Media:	"
		endif
	rcall   DEV_Init_Media_CMD
	rcall   DEV_GetErrorFlags_CMD
		ifdef	toc_debug
	rcall	UART_WriteHex
		endif

	cpi     param1, 0
	breq    TOC_noInitMedia_error

	dec     tmpH
	cpi     tmpH, 0
	brne    TOC_InitMedia   

	rjmp	TOC_Done
		
TOC_noInitMedia_error:
		
	ldi 	param1,0
	ldi 	param2,0
	ldi	param3,17
	mov	param4, param3	        ; can not be loaded immediate
	ldi	param3,0
	
	ldi	ZL, lo(TOC_Data)
	ldi 	ZH, hi(TOC_Data)
	

	ldi     tmpH, 3

TOC_ReadFirstSector:   

		ifdef	toc_debug
	UART_PRINT "\n\rReadSector1:	"
	push	param1
	rcall	UART_WriteHex
	mov	param1,param2
	rcall	UART_WriteHex
	mov	param1,param3
	rcall	UART_WriteHex
	mov	param1,param4
	rcall	UART_WriteHex
	pop	param1
		endif

	push	param1
	rcall	DEV_ReadSector_CMD
	rcall   DEV_GetErrorFlags_CMD
	mov	temp1, param1		; XXX
	pop	param1
	
	cpi     temp1, 0		; XXX
	breq    TOC_FirstSectorRead

	dec     tmpH
	cpi     tmpH, 0
	brne    TOC_ReadFirstSector

	rjmp	TOC_Done
	
TOC_FirstSectorRead:      
	
	ldi	temp1,0			; set level to zero

	ldi	ZL, lo(TOC_Data)	; set X-register to databuffer
	ldi	ZH, hi(TOC_Data)

	adiw	ZL, 63			; move to data[158]
	adiw	ZL, 63			; (start sector information)
	adiw	ZL, 32


	ld	temp2, Z		; get start-sector of root

	
	adiw	ZL, 9			; move to data[167]
	ld	temp3, Z		; (root directory length)

	lsr	temp3			; calculate data[167] mod 8
	lsr	temp3
	lsr	temp3

	ldi	ZL, lo(TOC_SubDirStart)	; go to start sector table
	ldi	ZH, hi(TOC_SubDirStart)	 

	ldi	temp4, 0
	st	Z+, temp2		; write start sector
	st	Z+, temp4
	st	Z+, temp4
	st	Z, temp4
	
	ldi	ZL, lo(TOC_SubDirLength); go to directory length array
	ldi	ZH, hi(TOC_SubDirStart)	 	

	st	Z,temp3
	

TOC_NewLevel:	

	
	ldi	XH, hi(TOC_SubDirCursor); RAM-Pointer to array directory cursors
	ldi	XL, lo(TOC_SubDirCursor)

	add	XL, temp1		; go to index of level
	add	XL, temp1

	
	ldi	YH, hi(TOC_DATA)
	ldi	YL, lo(TOC_DATA)
	mov	temp4, temp1
	
TOC_Loop1:	
	cpi	temp4, 0		; calculate high byte
	breq	TOC_ExitLoop1		; for 2048*level
	ldi  	temp3, 8
	add	YH, temp3
	dec	temp4
	rjmp	TOC_Loop1
TOC_ExitLoop1:

	adiw	YL, 63			; Skip 68 bytes of each sector
	adiw	YL, 5
	st	X+, YH
	st	X, YL

TOC_ProcessSubDir:

	ldi	ZL, lo(TOC_SubDirStart)	; go to start sector table
	ldi	ZH, hi(TOC_SubDirStart)	 
	
	add	ZL, temp1		; go to index of level
	add	ZL, temp1
	add	ZL, temp1
	add 	ZL, temp1
	
	ld 	param4, Z+
	ld	param3, Z+
	ld 	param2, Z+
	ld	param1, Z

	ldi	ZL, lo(TOC_Data)
	ldi 	ZH, hi(TOC_Data)
	mov	temp4, temp1
TOC_Loop4:	
	cpi	temp4,0			; calculate high byte
	breq	TOC_ExitLoop4		; for 2048*level
	ldi 	temp3, 8
	add	ZH, temp3
	dec	temp4
	rjmp	TOC_Loop4
TOC_ExitLoop4:

	ldi     tmpH, 3

TOC_ReadNewSector:  
	cpi	param1,0
	brne	TOC_Malformed_Sektor
	cpi	param2,0
	brne	TOC_Malformed_Sektor
	rjmp	TOC_NoMalformed_Sektor

TOC_Malformed_Sektor:
	rjmp	TOC_Done
	
TOC_NoMalformed_Sektor:	
	
		ifdef	toc_debug
	UART_PRINT "\n\rReadSector2:	"
	push	param1
	rcall	UART_WriteHex
	mov	param1,param2
	rcall	UART_WriteHex
	mov	param1,param3
	rcall	UART_WriteHex
	mov	param1,param4
	rcall	UART_WriteHex
	pop	param1
		endif
	
	push	param1	
	rcall	DEV_ReadSector_CMD
	rcall   DEV_GetErrorFlags_CMD
	mov	XL, param1		; XXX
	pop	param1
	
	cpi     XL, 0			; XXX
	breq    TOC_NewSectorRead

	dec     tmpH
	cpi     tmpH, 0
	brne    TOC_ReadNewSector

	rjmp	TOC_Done
	
TOC_NewSectorRead:
	
	
TOC_ProcessEntries:

	
	ldi	XL, lo(TOC_SubDirCursor); RAM-pointer to array 
	ldi	XH, hi(TOC_SubDirCursor); of subdirectory cursors
	
	add	XL, temp1		; go to index of level
	add	XL, temp1
	
	ld	temp5, X+		; get subdirectory cursor
	ld	temp6, X
	
	mov	YH, temp5		; and put in Y-register
	mov	YL, temp6

	ld	temp3, Y
	cpi	temp3, 0
	brne	TOC_SectorNotDone

	rjmp 	TOC_SectorDone

TOC_SectorNotDone:

	ldi	XL, lo(TOC_SubDirCursor); RAM-pointer to array 
	ldi	XH, hi(TOC_SubDirCursor); of subdirectory cursors


TOC_ReadEntry:

	ldi	tmpH, 0

	ldi	ZL, lo(TOC_Current)	; RAM-pointer for current
	ldi	ZH, hi(TOC_Current)	; entry data

	ldi	XL, lo(TOC_SubDirCursor); RAM-pointer to array 
	ldi	XH, hi(TOC_SubDirCursor); of subdirectory cursors

	add	XL, temp1		; go to index of level
	add	XL, temp1

	ld	temp5, X+		; get subdirectory cursor
	ld	temp6, X

	mov	YH, temp5		; and put in Y-register
	mov	YL, temp6

	adiw	YL, 2			; fetch start_sektor
	ld	temp5, Y+		; and append it to current
	st	Z+, temp5		; entry
	ld	temp5, Y+
	st	Z+, temp5
	ld	temp5, Y+
	st	Z+, temp5
	ld	temp5, Y+
	st	Z+, temp5

	adiw	YL, 4			; fetch length of directory
	ld	temp5, Y+		; or file and append it to
	st	Z+, temp5		; current entry
	ld	temp5, Y+
	st	Z+, temp5
	ld	temp5, Y+
	st	Z+, temp5
	ld	temp5, Y+
	st	Z+, temp5

	adiw	YL, 11			; fetch directory flag 
;	lds	temp4, TOC_Flags        ; additional flags
	ld	temp4, Y
;	bst	temp5, 1		; take bit 1
;	bld	temp4, 7		; and store it as bit 7
	st	Z+, temp4		; finally store it

	sbrc	temp4, 1
	ldi	tmpH, 4
	
	adiw	YL,7			; fetch entry_Length
	ld	temp2, Y+
	;sbrs	temp4, 1		; if not a directory entry
	;subi	temp2, 4		; decrease length by 4

	mov	param3, temp2
	
	adiw	YL,1			; to get to start of filename
	
	
	ldi	tmpL, 0
	
TOC_ReadEntryLoop:
	
	ld	temp5, Y+		; get char

	cpi	tmpL, 60
	brge	TOC_SkipChar
	st	Z+, temp5		; append it

TOC_SkipChar:

	mov	param2, temp5

	cpi	tmpH, 4
	breq	TOC_MP3Recognition_Done

	cpi	param2, 0x2e
	brne	TOC_noPoint
	ldi	tmpH, 1
	rjmp	TOC_MP3Recognition_Done

TOC_noPoint:
	cpi	tmpH, 1
	brne	TOC_not1
	cpi	param2, 0x4d
	breq	TOC_1
	cpi	param2, 0x6d
	brne	TOC_not1
TOC_1:
	inc	tmpH
	rjmp 	TOC_MP3Recognition_Done
	
TOC_not1:
	cpi	tmpH, 2
	brne	TOC_not2
	cpi	param2, 0x50
	breq	TOC_2
	cpi	param2, 0x70
	brne	TOC_not2
TOC_2:
	inc	tmpH
	rjmp 	TOC_MP3Recognition_Done

TOC_not2:
	cpi	tmpH, 3
	brne	TOC_not3
	;cpi	param2, 0x33
	;brne	TOC_not3
	inc	tmpH

TOC_not3:

TOC_MP3Recognition_Done:

	adiw	YL,1			; skip padding byte

	inc 	tmpL
	dec	temp2			; decrease loop variable
	dec 	temp2
	cpi	temp2,0			; and test loop condition
	breq	TOC_ReadEntryLoopExit
	rjmp	TOC_ReadEntryLoop

TOC_ReadEntryLoopExit:
	

	ldi	temp2, 120
	cpi	param3, 120 
	brge	TOC_ReadEntryLoop_Exit
	
	sub	temp2, param3

	push	temp1
	ldi	temp1, 0x20
	mov	temp5, temp1
	pop	temp1


TOC_ReadEntryLoop2:

	st	Z+, temp5
	dec	temp2
	dec	temp2
	cpi	temp2,0
	brne	TOC_ReadEntryLoop2

TOC_ReadEntryLoop_Exit:
	
	push	param1
	
	ldi	ZL, lo(TOC_current)
	ldi	ZH, hi(TOC_current)
	

	ldi	temp2,9

	cpi	tmpH, 4
	brne	TOC_Output_done

	UART_Print "TOC"
	mov	param1, temp1
	rcall	UART_Write
	
TOC_Output_Loop1:

	ld	param1,Z+
	rcall	UART_Write
	dec  	temp2

	cpi	temp2,0
	brne	TOC_Output_Loop1

	ldi	temp2,60	

TOC_Output_Loop2:

	ld	param1,Z+
	rcall	UART_Write
	dec  	temp2
	cpi	temp2,0
	brne	TOC_Output_Loop2

TOC_Output_done:
	
	pop	param1	


	;sbrs	temp4,1			; correct lack caused by file
	;adiw	YL,4			; entry
	
	ldi	XL, lo(TOC_SubDirCursor); RAM-pointer to array 
	ldi	XH, hi(TOC_SubDirCursor); of subdirectory cursors

	add	XL, temp1		; go to index of level
	add	XL, temp1

	st	X+, YH			; store new cursor position
	st	X, YL
	
	ldi	ZL, lo(TOC_Current)	; RAM-pointer for current
	ldi	ZH, hi(TOC_Current)	; entry data

	adiw	ZL, 8			; go to directory flag
	ld	temp4, Z		; fetch it

	sbrc	temp4, 1
	rjmp	TOC_direntry		; if not directory continue processing
	rjmp	TOC_ProcessEntries

TOC_direntry:

	inc	temp1			; else increase level

	cpi	temp1, 9	; if level exceeds 9, do
	brge	TOC_MaxLevel	; not proceed subdirectory
	rjmp	TOC_NoMaxLevel
	
TOC_MaxLevel:
	rjmp	TOC_LevelDone

TOC_NoMaxLevel:	
	
	ldi	XL, lo(TOC_SubDirStart)	; RAM-pointer to array
	ldi	XH, hi(TOC_SubDirStart)	; of start sectors

	mov 	temp2, temp1
	add	temp2, temp1
	add 	temp2, temp1
	add 	temp2, temp1
	
	add	XL, temp2		; go to index of level
	ldi	temp2, 0
	adc	XH, temp2		; (4 bytes each entry)
	
	sbiw	ZL, 8			; back to the start

	ld	temp5, Z+		; copy the start sector
	st	X+, temp5		; to array
	ld	temp5, Z+
	st	X+, temp5
	ld	temp5, Z+
	st	X+, temp5
	ld	temp5, Z+
	st	X+, temp5
	
	adiw	ZL, 1			
	
	ld	temp5, Z		; fetch directory length
	lsr	temp5			; mod 8
	lsr	temp5
	lsr	temp5
	
	ldi	XL, lo(TOC_SubDirLength); RAM pointer to start of array
	ldi	XH, hi(TOC_SubDirLength); with length of subdir

	ldi 	temp2, 0
	add	XL, temp1		; go to index of level
	adc	XH, temp2

	st	X, temp5		; store length

	rjmp	TOC_NewLevel
	
TOC_SectorDone:

	ldi	XL, lo(TOC_SubDirLength); RAM pointer to start of array
	ldi	XH, hi(TOC_SubDirLength); with length of subdir

	ldi	temp2, 0
	add	XL, temp1		; go to index of level
	adc	XH, temp2

	ld	temp4, X		; fetch length
	dec	temp4			; decrease it

	st	X, temp4		; and write it back

;  	rjmp	TOC_LevelDone	
	cpi	temp4, 0		; entire subdir read ?
	brne	TOC_LevelNotDone	; then level is complete

	rjmp	TOC_LevelDone

TOC_LevelNotDone:	
	ldi	XL, lo(TOC_SubDirStart)	; RAM-pointer to array
	ldi	XH, hi(TOC_SubDirStart)	; of start sectors

	
	mov 	temp2, temp1
	add	temp2, temp1
	add 	temp2, temp1
	add 	temp2, temp1
;  	push	temp1
;  	ldi	temp1,3
;  	add	temp2, temp1
;  	pop	temp1
	
	add	XL, temp2		; go to index of level
	ldi	temp2, 0
	adc	XH, temp2		; (4 bytes each entry)
	
	ld	temp2, X		; fetch entry and increase by one
	ldi	temp3, 1		; especially take care of
	add	temp2, temp3		; the carry flag
	st	X+, temp2
	ldi	temp3, 0
	ld	temp2, X
	adc	temp2, temp3
	st	X+, temp2
	ld	temp2, X
	adc	temp2, temp3
	st	X+, temp2
	ld	temp2, X
	adc	temp2, temp3
	st	X, temp2

	ldi	XL, lo(TOC_SubDirCursor); RAM-pointer to array 
	ldi	XH, hi(TOC_SubDirCursor); of subdirectory cursors

	add	XL, temp1		; go to index of level
	add	XL, temp1

	ldi	YH, hi(TOC_Data)
	ldi	YL, lo(TOC_Data)
	mov	temp4, temp1

TOC_Loop2:	

	cpi	temp4,0			; calculate high byte
	breq	TOC_ExitLoop2		; for 2048*level
	ldi 	temp3, 8
	add	YH, temp3
	dec	temp4
	rjmp	TOC_Loop2

TOC_ExitLoop2:

	st	X+, YH
	st	X, YL
	
	rjmp	TOC_ProcessSubDir	; continue with next sector
	
TOC_LevelDone:

	cpi	temp1, 0		; all levels done ?
	breq	TOC_Done		; then exit

	dec	temp1			; else decrease level
	rjmp	TOC_ProcessEntries	; and proceed

TOC_Done:

	UART_Print "END"
	
	pop	param1
	pop	param2
	pop	param3
	pop	param4
	pop	tmpH
	pop	tmpL
	pop	temp6			; restore temp values
	pop	temp5
	pop	temp4
	pop	temp3
	pop	temp2
	pop	temp1
	pop	ZH
	pop	ZL
	pop	YH
	pop	YL
	pop	XH
	pop	XL
	sei

  ifdef	toc_debug
	UART_PRINT "\nE:T_Read"
  endif
	ret

	
;***********************************************************************

	endif

;*
;* $Log: tocj.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
