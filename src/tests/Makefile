CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: dataProcessingImmTest

dataProcessingImmTest: dataProcessingImmTest.o ../emulator/machine.c  ../emulator/memory.c  ../emulator/instructions/dataProcessingImm.c 

clean:
	$(RM) *.o dataProcessingImmTest
	
