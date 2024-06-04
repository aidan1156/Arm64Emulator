CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.c ./assembler/maps.c ./assembler/utilities.c  ./assembler/fileIO.c ./assembler/instructions/dataProcessingArith.c ./assembler/instructions/dataProcessingImm.c ./assembler/instructions/dataProcessingReg.c
emulate: emulate.o ./emulator/memory.c ./emulator/machine.c ./emulator/utilities.c ./emulator/instructions/dataProcessingImm.c ./emulator/instructions/dataProcessingReg.c ./emulator/instructions/sdt.c ./emulator/instructions/branchInstr.c

clean:
	$(RM) *.o assemble emulate
	
