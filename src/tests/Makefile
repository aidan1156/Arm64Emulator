CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: dataProcessingImmTest dataProcessingRegTest sdtTest branchInstrTest assemblerBranchInstrTest

dataProcessingImmTest: ../emulator/machine.c ../emulator/utilities.c  ./dataProcessingImmTest.c ../emulator/memory.c  ../emulator/instructions/dataProcessingImm.c 

dataProcessingRegTest: ../emulator/machine.c ../emulator/utilities.c  ./dataProcessingRegTest.c ../emulator/memory.c  ../emulator/instructions/dataProcessingReg.c  ../emulator/instructions/dataProcessingImm.c

sdtTest: ../emulator/machine.c ../emulator/utilities.c  ../emulator/memory.c ../emulator/instructions/dataProcessingImm.c ../emulator/instructions/sdt.c ./sdtTest.c

branchInstrTest: ../emulator/machine.c ./branchInstrTest.c ../emulator/memory.c ../emulator/instructions/branchInstr.c

assemblerBranchInstrTest: ../assembler/maps.c ./assemblerBranchInstrTest.c ../assembler/utilities.c ../assembler/instructions/branchInstr.c

clean:
	$(RM) *.o dataProcessingImmTest dataProcessingRegTest sdtTest branchInstrTest
