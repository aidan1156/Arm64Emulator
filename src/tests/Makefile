CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: dataProcessingImmTest dataProcessingRegTest sdtTest

dataProcessingImmTest: ../emulator/machine.c  ./dataProcessingImmTest.c ../emulator/memory.c  ../emulator/instructions/dataProcessingImm.c

dataProcessingRegTest: ../emulator/machine.c  ./dataProcessingRegTest.c ../emulator/memory.c  ../emulator/instructions/dataProcessingReg.c  ../emulator/instructions/dataProcessingImm.c

sdtTest: ../emulator/machine.c  ../emulator/memory.c ../emulator/instructions/dataProcessingImm.c ../emulator/instructions/sdt.c ./sdtTest.c

clean:
	$(RM) *.o dataProcessingImmTest sdt
	
