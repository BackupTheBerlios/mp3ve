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
;* $Id: tocentry.inc,v 1.1 2002/07/31 17:31:59 dressler Exp $
;***********************************************************************
;*
;*  Title           : TOC Entry
;*  Version         : 2.0
;*
;*  Filename        : tocentry.inc
;*  Created         : 20.01.2001 by Marc Dressler
;*  Last changed    : 08.04.2001 by Marc Dressler
;*
;*  Target-uC       : Atmel AVR AT90Sxxxx
;*  Target-PCB      : MP3IDE V2.0
;*  Description     : -
;*
;*  Homepage        : www.MP3ve.de
;*		      www.uC-Elektronik.de
;*                    www.Hardware-Schmiede.de	
;*
;*  Uses            : -
;*
;***********************************************************************

	ifndef	tocentryinc	; prevent multiple binding

	SEGMENT DATA

TocEntry:
TocE_start:
TocE_id:	res	1		; ID: 
					; 0x00 - Volume informations
					; 0x01 - TOC Entry
					; 0x02 - End of TOC reached
					; 0xff - error reading toc
TocE_level:	res	1
TocE_flags1:	res	1	

					; TocE_flags1: [default]
					; misc.Flags
					; bit 0 [0] - media LSB
					; bit 1 [0] - media ...
					; bit 2 [0] - media ...
					; bit 3 [0] - media MSB
					; bit 4 [0] - master (0) / slave (1)
					; bit 5 [0] - cd (0) / hdd (1)
					; bit 6 [0] - mp3file (0) / audiotrack (1)
					; bit 7 [0] - file (0) / directory (1)
TocE_flags2:	res	1	
					; TocE_flags2: [default]
					; misc.Flags - reserved
					; bit 0 [0] - 
					; bit 1 [0] - 
					; bit 2 [0] - 
					; bit 3 [0] - 
					; bit 4 [0] - 
					; bit 5 [0] - 
					; bit 6 [0] -
					; bit 7 [0] -
					

TocE_startsector:	
		res	4	; start sector of file (MSB..LSB)
TocE_length:	res	4	; length in bytes (MSB..LSB)

TocE_ID3:			; start of ID3-Tag
; TocE_Tag:	res	3	; always "TAG"
TocE_title:	res	30	
TocE_interpret:	res	30
TocE_album:	res	30
TocE_year:	res	4
TocE_comment:	res	28
TocE_trackpresent:	res	1	; 0 if TocE_track = "track" else "comment"
TocE_track:	res	1
TocE_genre:	res	1
TocE_end:


;ID3-Tag v1.0 
;
;Offset	Bytes	Beschreibung 
;0	3	Kennzeichnung ("TAG")
;3	30	Titel 
;33	30	Interpret 
;63	30	Album 
;93	4	Jahr 
;97	30	Kommentar 
;127	1	Genre 

;ID3-Tag v1.1 
;Offset	Bytes	Beschreibung 
;0	3	Kennzeichnung 
;3	30	Titel 
;33	30	Künstler 
;63	30	Album 
;93	4	Jahr 
;97	28	Kommentar 
;125	2	Album-Track 
;127	1	Genre 


;Genre - ID3 v1.0 Standard: 
;
;
;   1.Blues 
;   2.Classic Rock 
;   3.Country 
;   4.Dance 
;   5.Disco 
;   6.Funk 
;   7.Grunge 
;   8.Hip-Hop 
;   9.Jazz 
;  10.Metal 
;  11.New Age 
;  12.Oldies 
;  13.Other 
;  14.Pop 
;  15.R&B 
;  16.Rap 
;  17.Reggae 
;  18.Rock 
;  19.Techno 
;  20.Industrial 
;  21.Alternative 
;  22.Ska 
;  23.Death Metal 
;  24.Pranks 
;  25.Soundtrack 
;  26.Euro-Techno 
;  27.Ambient 
;  28.Trip-Hop 
;  29.Vocal 
;  30.Jazz+Funk 
;  31.Fusion 
;  32.Trance 
;  33.Classical 
;  34.Instrumental 
;  35.Acid 
;  36.House 
;  37.Game 
;  38.Sound Clip 
;  39.Gospel 
;  40.Noise 
;  41.AlternRock 
;  42.Bass 
;  43.Soul 
;  44.Punk 
;  45.Space 
;  46.Meditative 
;  47.Instrumental Pop 
;  48.Instrumental Rock 
;  49.Ethnic 
;  50.Gothic 
;  51.Darkwave 
;  52.Techno-Industrial 
;  53.Electronic 
;  54.Pop-Folk 
;  55.Eurodance 
;  56.Dream 
;  57.Southern Rock 
;  58.Comedy 
;  59.Cult 
;  60.Gangsta 
;  61.Top 40 
;  62.Christian Rap 
;  63.Pop/Funk 
;  64.Jungle 
;  65.Native American 
;  66.Cabaret 
;  67.New Wave 
;  68.Psychadelic 
;  69.Rave 
;  70.Showtunes 
;  71.Trailer
;  72.Lo-Fi 
;  73.Tribal 
;  74.Acid Punk 
;  75.Acid Jazz 
;  76.Polka 
;  77.Retro 
;  78.Musical 
;  79.Rock & Roll 
;  80.Hard Rock 
;

;In Winamp neu eingeführte Genres: 
;
;  81.Folk 
;  82.Folk/Rock 
;  83.National Folk 
;  84.Swing 
;  85.Bebob 
;  86.Latin 
;  87.Revival 
;  88.Celtic 
;  89.Bluegrass 
;  90.Avantgarde 
;  91.Gothic Rock 
;  92.Progressive Rock 
;  93.Psychedelic Rock 
;  94.Symphonic Rock 
;  95.Slow Rock 
;  96.Big Band 
;  97.Chorus 
;  98.Easy Listening 
;  99.Acoustic 
; 100.Humour 
; 101.Speech 
; 102.Chanson 
; 103.Opera 
; 104.Chamber Music 
; 105.Sonata 
; 106.Symphony 
; 107.Booty Bass 
; 108.Primus 
; 109.Porn Groove 
; 110.Satire 
; 111.Slow Jam 
; 112.Club 
; 113.Tango 
; 114.Samba 
; 115.Folklore 
; 116.Ballad 
; 117.Power Ballad 
; 118.Rhythmic Soul 
; 119.Freestyle 
; 120.Duet 
; 121.Punk Rock 
; 122.Drum Solo 
; 123.A capella 
; 124.Euro-House 
; 125.Dance Hall 
;

;***********************************************************************

	endif

;*
;* $Log: tocentry.inc,v $
;* Revision 1.1  2002/07/31 17:31:59  dressler
;* initial checkin
;*
