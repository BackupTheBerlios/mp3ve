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
;* $Id: bitfuncs.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : BITFUNCS
;*  Version         : AS 1.39
;*
;*  Filename        : bitfuncs.inc
;*  Created         : 30.05.1993 by AS
;*  Last changed    : 29.08.2000 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : all
;*  Description     : functions for bitmanipulation
;*
;*  Homepage        : www.MP3ve.de
;*                    www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************

                save
                listing off   ; kein Listing ueber diesen File

		ifndef   bitfuncsinc    ; verhindert Mehrfacheinbindung

bitfuncsinc     equ      1

		if	 mompass=1
		 message "Standardbitfunktionen (C) 1993 Alfred Arnold"
		endif

;----------------------------------------------------------------------------
; erstmal einige Unterfunktionen:

; liefert eine Maske in der ab 'start' 'bits' Bit gesetzt sind, z.B. zum
; Ausmaskieren einzelner Bits:

mask            function start,bits,((1<<bits)-1)<<start

; das gleiche invers zum Loeschen von Bitgruppen:

invmask         function start,bits,~mask(start,bits)

; liefert die Bits 'start' bis 'start'+'bits'-1 aus 'x':

cutout          function x,start,bits,x&mask(start,bits)

;----------------------------------------------------------------------------
; einige häufig gebrauchte Sondermasken:

; oberes Byte eines 16-Bit-Wortes:

hi              function x,(x>>8)&255

; dito unteres Byte:

lo              function x,x&255

; obere Hälfte aus einem 32-Bit-Wort:

hiword          function x,(x>>16)&65535

; dito untere Hälfte:

loword          function x,x&65535

; Boolean-Funktionen, ob eine Zahl gerade oder ungerade ist:

odd             function x,(x&1)=1
even            function x,(x&1)=0

; liefert Bit 'n' aus 'x':

getbit          function x,n,(x>>n)&1

;----------------------------------------------------------------------------
; Schiebefunktionen:

; Schieben eines 'size'-Bit-Wortes um 'n' Stellen nach links oder rechts:

shln            function x,size,n,(x<<n)&mask(0,size)
shrn            function x,size,n,(x>>n)&mask(0,size-n)

; Rotation eines 'size'-Bit-Wortes um 'n' Stellen nach links oder rechts;
; der erste Term läßt die restlichen Bits unverändert und kann bei Bedarf
; gelöscht werden:

rotln           function x,size,n,cutout(x,size,32-size)|shln(x,size,n)|shrn(x,size,size-n)
rotrn           function x,size,n,cutout(x,size,32-size)|shrn(x,size,n)|shln(x,size,size-n)

;***********************************************************************
	endif
	restore                 ; wieder erlauben


;*
;* $Log: bitfuncs.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
