CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate flappyBird

assemble: assemble.c
emulate: emulate.c ./emulator/memory.c ./emulator/machine.c ./emulator/utilities.c ./emulator/instructions/dataProcessingImm.c ./emulator/instructions/dataProcessingReg.c ./emulator/instructions/sdt.c ./emulator/instructions/branchInstr.c

flappyBird: ./extension/flappyBird/flappyBird.c ./extension/gameEngine.c ./extension/flappyBird/Pipes.c ./extension/flappyBird/Bird.c ./extension/flappyBird/text.c
	$(CC) $(CFLAGS) -o flappyBird ./extension/flappyBird/flappyBird.c ./extension/gameEngine.c ./extension/flappyBird/Pipes.c ./extension/flappyBird/Bird.c ./extension/flappyBird/text.c

clean:
	$(RM) *.o assemble emulate flappyBird
	
