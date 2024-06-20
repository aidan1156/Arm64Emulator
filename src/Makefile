CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate flappyBird 2048

assemble: assemble.c
emulate: emulate.c ./emulator/memory.c ./emulator/machine.c ./emulator/utilities.c ./emulator/instructions/dataProcessingImm.c ./emulator/instructions/dataProcessingReg.c ./emulator/instructions/sdt.c ./emulator/instructions/branchInstr.c

flappyBird: ./extension/flappyBird/flappyBird.c ./extension/gameEngine.c ./extension/flappyBird/Pipes.c ./extension/flappyBird/Bird.c ./extension/flappyBird/text.c
	$(CC) $(CFLAGS) -o flappyBird ./extension/flappyBird/flappyBird.c ./extension/gameEngine.c ./extension/flappyBird/Pipes.c ./extension/flappyBird/Bird.c ./extension/flappyBird/text.c

2048: ./extension/2048/2048.c ./extension/gameEngine.c ./extension/2048/Block.c
	$(CC) $(CFLAGS) -o 2048 ./extension/2048/2048.c ./extension/gameEngine.c ./extension/2048/Block.c

clean:
	$(RM) *.o assemble emulate flappyBird 2048
	
