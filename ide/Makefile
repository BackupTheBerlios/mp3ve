# Makefile
# !! preversion !!
#

ASM = mp3veide.asm
INC = mp3.inc spi.inc sta.inc

AS = asl -x -L -i inc/ 
HEX = p2hex -F intel 
BIN = p2bin 

mp3ide15.bin: 
#	rm burn.hex
	$(AS) asm/mp3veide.asm
	$(HEX) asm/mp3veide.p
	$(BIN) asm/mp3veide.p
	cp asm/mp3veide.hex mp3veide.hex

all: mp3veide.bin

clean:
	rm asm/mp3veide.lst
	rm asm/mp3veide.p
	rm asm/mp3veide.hex
	rm asm/mp3veide.bin
	rm mp3veide.hex
	rm stapatch.hex
	rm ISP.LOG

patch:
	$(AS) asm/stapatch.asm
	$(HEX) asm/stapatch.p
	$(BIN) asm/stapatch.p
	cp asm/stapatch.hex stapatch.hex



