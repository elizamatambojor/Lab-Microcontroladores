FAMILY=-mpic14
MCU=-p12f683
CC=sdcc
LD=sdcc
CFLAGS=-I. -I/usr/local/share/sdcc/non-free/include
TARGET=Dado_L1 

SRCS = Dado_L1.c  

all:
	${CC} --use-non-free ${FAMILY} ${MCU} ${CFLAGS} -o ${TARGET} ${SRCS}

clean:
	rm -f *.c~ *.h~ *.o *.elf *.hex *.asm