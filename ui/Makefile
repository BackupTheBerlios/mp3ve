        MCU = at90s8515

        OBJ = object/spi.o object/lcd.o object/uart.o \
              object/main.o object/rpc.o object/toc.o \
              object/mp3.o object/fsm.o object/bitfunction.o \
              object/dev.o object/sys.o

        INCFLAGS = -Iinclude
        CFLAGS = -c -g -O2 -Wstrict-prototypes -Wall #-Wa,-ahlms=$(<:.c=.lst)
        PFLAGS = -mmcu=$(MCU)
        LDFLAGS = -Wl,--script,lib/linker.x

        CC = avr-gcc
        BIN = avr-objcopy
        RM = rm -f

UserInterface: $(OBJ)
	$(CC) $(PFLAGS) $(LDFLAGS) -o main.elf $(OBJ)
	$(BIN) -O srec main.elf rom/main.rom
	rm -f main.elf

object/main.o : source/main.c
#	$(CC) $(CFLAGS) -Wa,-ahlms=main.lst $(INCFLAGS) $(PFLAGS) -o object/main 
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/main.o source/main.c 

object/spi.o : include/spi.h source/spi.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/spi.o source/spi.c

object/lcd.o : include/lcd.h include/spi.h source/lcd.c source/spi.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/lcd.o source/lcd.c

object/uart.o : include/uart.h source/uart.c
#	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -Wa,-ahlms=uart.lst -o object/uart.o source/uart.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/uart.o source/uart.c

object/fsm.o: include/fsm.h source/fsm.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/fsm.o source/fsm.c

object/bitfunction.o: include/bitfunction.h source/bitfunction.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/bitfunction.o source/bitfunction.c

object/rpc.o : include/rpc.h source/rpc.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/rpc.o source/rpc.c

object/sys.o : include/sys.h source/sys.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/sys.o source/sys.c

object/mp3.o : include/mp3.h source/mp3.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/mp3.o source/mp3.c

object/toc.o : include/toc.h source/toc.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/toc.o source/toc.c

object/dev.o : include/dev.h source/dev.c
	$(CC) $(CFLAGS) $(INCFLAGS) $(PFLAGS) -o object/dev.o source/dev.c      

clean:
	$(RM) $(OBJ)
	$(RM) rom/*




