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
;* $Id: convert.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title       : convert
;*  Version     : 1.5
;*
;*  Filename    : convert.inc
;*  Created     : 13.01.2000 by Jens Funke
;*  Last changed: 13.12.2000 by Marc Dressler
;*
;*  Target-uC   : Atmel AVR AT90Sxxxx
;*  Target-PCB  : all
;*  Description : Convert funktion. Bin2Hex, Hex2Bin, Bin2ASCII, ASCII2Bin
;*                Hex2ASCII and ASCII2Hex
;*
;*  Homepage    : www.MP3ve.de
;*                www.uC-Elektronik.de
;*                www.Hardware-Schmiede.de	
;*
;*  Uses        : -
;*
;***********************************************************************

   	ifndef  convertinc       ; prevent multiple binding
convertinc       equ     1

                if       mompass=1
                 message "Convert Include-File V1.5 (C) 2000 by Jens Funke"

                endif

  ifdef include_convert

	SEGMENT CODE

;***********************************************************************
;* FUNCTION     : Bin2Hex
;*
;* CREATED      : 13.01.2000  by Jens Funke
;*
;* LAST CHANGED : 13.01.2000  by Jens Funke
;*
;* DESCRIPTION  : Convert Binary to Hex. The 8 bits of the Binary value
;*                are saved in 8 byte. MSB is represented by the first byte.
;*
;* INPUT        : ZH = HI(Pointer) to Startaddress (MSB)
;*                ZL = LO(Pointer) to Startaddress
;*
;* OUTPUT       : Param1 = Converted Bin value as Hex value
;*
;***********************************************************************
CONV_Bin2Hex:
        push    temp1
        push 	temp2

        ldi     param1,0
        ldi     temp2,0x80

Loop_Bin2Hex:
        cpi     temp2,0                ;end after 8 loops
        breq    End_Bin2Hex
        ld      temp1,Z+                ;load indirect from Z
        cpi     temp1,0
        breq    Skip_Bin2Hex            ;if Byte = 0 then step over
        add     param1,temp2           ;    else param1 = param1 + temp2
Skip_Bin2Hex:
        lsr     temp2                  ;shift right
        rjmp    Loop_Bin2Hex

End_Bin2Hex:
	pop	temp2
        pop     temp1
        ret

;***********************************************************************
;* FUNCTION     : Hex2Bin
;*
;* CREATED      : 13.01.2000  by Jens Funke
;*
;* LAST CHANGED : 05.05.2000  by Jens Funke
;*
;* DESCRIPTION  : Convert Hex to Binary
;*
;* INPUT        : ZH = HI(Pointer) to Startaddress (MSB)
;*                ZL = LO(Pointer) to Startaddress
;*                Param3 = Hex value which will converted to Bin.
;*
;* OUTPUT       : ZH = HI(Pointer) to Startaddress (MSB) same to INPUT
;*                ZL = LO(Pointer) to Startaddress
;*
;***********************************************************************
CONV_Hex2Bin:
        push    temp1
        push    temp2
        push	param3
        push    ZL
        push    ZH

        ldi      temp1,8                 ;init loopcounter

Loop_Hex2Bin:
        subi    temp1,1
        rol     param3                  ;shift left
        ;LSB was shifted into the carry bit
        brcs    Set_Hex2Bin             ;branch if Carry set
;       brcc    Clear_Hex2Bin           ;branch if Carry cleared
;Clear_Hex2Bin:
        ldi     temp2,0
        rjmp    Go_Hex2Bin
Set_Hex2Bin:
        ldi     temp2,1
;       rjmp    Go_Hex2Bin

Go_Hex2Bin:
        st      Z+,temp2                ;store indirect
        cpi     temp1,0
        brne    Loop_Hex2Bin             ;end after 7 loops

        pop     ZH
        pop     ZL
        pop	param3
        pop     temp2
        pop     temp1
        ret

;***********************************************************************
;* FUNCTION     : Bin2Ascii
;*
;* CREATED      : 13.01.2000  by Jens Funke
;*
;* LAST CHANGED : 05.05.2000  by Jens Funke
;*
;* DESCRIPTION  : Convert Binary to ASCII.
;*                01100100b to 0,1,1,0,0,1,0,0
;*
;* INPUT        : ZH = HI(Pointer) to Startaddress (MSB)
;*                ZL = LO(Pointer) to Startaddress
;*                Param3 = Amount of bits which will be converted
;*
;* OUTPUT       : ZH = HI(Pointer) to Startaddress (MSB) same to INPUT
;*                ZL = LO(Pointer) to Startaddress
;*
;***********************************************************************
CONV_Bin2Ascii:
        push    temp2
        push    temp1
        push	param3
        push    ZL
        push    ZH

Loop_Bin2Ascii:
        cpi     param3,0                ;loop until (param3 = 0)
        breq    End_Bin2Ascii

        ld      temp1,Z                 ;don't increment Z, because the new value
        cpi     temp1,0                 ; will be written at the same position
        breq    Clear_Bin2Ascii
;       brne    Set_Bin2Ascii
;Set_Bin2Ascii:
        ldi     temp2,'1'               ;convert 1h to '1'
        rjmp    Go_Bin2Ascii
Clear_Bin2Ascii:
        ldi     temp2,'0'               ;convert 0h to '0'
        rjmp    Go_Bin2Ascii
Go_Bin2Ascii:
        st      Z+,temp2                ;overwrite the old with the new value
        dec     param3
        rjmp    Loop_Bin2Ascii

End_Bin2Ascii:
        ldi     temp1,0                 ;the last ASCII charakter must be 0
        st      Z+,temp1

        pop     ZH
        pop     ZL
        pop	param3
        pop     temp1
        pop     temp2
        ret

;***********************************************************************
;* FUNCTION     : Ascii2Bin
;*
;* CREATED      : 13.01.2000  by Jens Funke
;*
;* LAST CHANGED : 13.01.2000  by Jens Funke
;*
;* DESCRIPTION  : Convert ASCII.to Binary
;*                0,1,1,0,0,1,0,0 to 01100100b
;*
;* INPUT        : ZH = HI(Pointer) to Startaddress (MSB)
;*                ZL = LO(Pointer) to Startaddress
;*                Param3 = Value for undefined Binary values (0 or 1)
;*
;* OUTPUT       : ZH = HI(Pointer) to Startaddress (MSB) same to INPUT
;*                ZL = LO(Pointer) to Startaddress
;*                Param3 = Amount of undefined values
;*
;***********************************************************************
CONV_Ascii2Bin:
        push    temp1
        push    temp2
        push    ZL
        push    ZH

        ldi     temp1,0                 ;init ErrrorCounter

Loop_Ascii2Bin:
        ld      temp2,Z
        cpi     temp2,0                 ;end loop at the first 0
        breq    End_Ascii2Bin
        cpi     temp2,'0'               ;convert 0 to '0'
        breq    Clear_Ascii2Bin
        cpi     temp2,'1'               ;convert 1 to '1'
        breq    Set_Ascii2Bin
        rjmp    Unknown_Ascii2Bin       ;convert x to value of param3
Set_Ascii2Bin:
        ldi     temp2,1
        rjmp    Go_Ascii2Bin
Clear_Ascii2Bin:
        ldi     temp2,0
        rjmp    Go_Ascii2Bin
Unknown_Ascii2Bin:
        inc     temp1
        mov     temp2,param3
;       rjmp    Go_Ascii2Bin
Go_Ascii2Bin:
        st      Z+,temp2                ;overwrite old value with the new one
        rjmp    Loop_Ascii2Bin

End_Ascii2Bin:
        mov     param3,temp1            ;param3 contains the amount of undefined values

        pop     ZH
        pop     ZL
        pop     temp2
        pop     temp1
        ret

;***********************************************************************
;* FUNCTION     : Hex2Ascii
;*
;* CREATED      : 13.01.2000  by Jens Funke
;*
;* LAST CHANGED : 13.01.2000  by Jens Funke
;*
;* DESCRIPTION  : Convert Hex to ASCII. A5h to A,5
;*
;* INPUT        : Param1 = Hex value which will be converted
;*
;* OUTPUT       : Param1 = HighNibble of the Hex value as ASCII
;*                Param2 = LowNibble of the Hex value as ASCII
;*
;***********************************************************************
CONV_Hex2Ascii:
        push    temp3
        push    temp2
        push    temp1

        mov     temp1,param1
        andi    temp1,0b11110000        ;high nibble
        swap    temp1
        ldi     temp3,0x30
        add     temp1,temp3             ;add 30h if number

        cpi     temp1,0x3A
        brlo    SkipHigh_Hex2Ascii
        ldi     temp3,0x7
        add     temp1,temp3             ;add 7h again if charakter
SkipHigh_Hex2Ascii:
        mov     temp2,param1
        andi    temp2,0b00001111        ;low nibble
        ldi     temp3,0x30
        add     temp2,temp3             ;add 30h if number

        cpi     temp2,0x3A
        brlo    SkipLow_Hex2Ascii
        ldi     temp3,0x7
        add     temp2,temp3             ;add 7h again if charakter
SkipLow_Hex2Ascii:
        mov     param1,temp1            ;return HighNibble
        mov     param2,temp2            ;return LowNibble

        pop     temp1
        pop     temp2
        pop     temp3
        ret

;***********************************************************************
;* FUNCTION     : Ascii2Hex
;*
;* CREATED      : 13.01.2000  by Jens Funke
;*
;* LAST CHANGED : 13.01.2000  by Jens Funke
;*
;* DESCRIPTION  : Convert ASCII to Hex. A,F to AFh.
;*                ATTENTION: a-f are not allowed. Correctness is unchecked.
;*
;* INPUT        : Param1 = HighNibble of the Hex value as ASCII
;*                Param2 = LowNibble of the Hex value as ASCII
;*
;* OUTPUT       : Param1 = Hex value which was converted
;*
;***********************************************************************
CONV_Ascii2Hex:
        subi    param1,0x30             ;sub 30h always
        cpi     param1,0x10
        brlo    SkipHigh_Ascii2Hex
        subi    param1,0x7              ;sub 7 again if it was a charakter
SkipHigh_Ascii2Hex:
        subi    param2,0x30             ;sub 30h alwalys
        cpi     param2,0x10
        brlo    SkipLow_Ascii2Hex
        subi    param2,0x7              ;sub 7 again if it was a charakter
SkipLow_Ascii2Hex:
        swap    param1                  ;combine HighNibble and LowNibble to Hex value
        or      param1,param2
        ret

  endif	
;***********************************************************************

	endif

;*
;* $Log: convert.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
