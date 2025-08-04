OBJS = sdram.o clock.o console.o devices.o gfx.o lcd-spi.o

BINARY = main

# we use sin/cos from the library
LDLIBS += -lm

LDSCRIPT = ../stm32f429i-discovery.ld

include ../../Makefile.include
