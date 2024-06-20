CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate flappyBird pacman gameOfLife pong

assemble: assemble.c ./assembler/maps.c ./assembler/utilities.c  ./assembler/fileIO.c ./assembler/instructions/dataProcessingArith.c ./assembler/instructions/dataProcessingImm.c ./assembler/instructions/dataProcessingReg.c ./assembler/instructions/branchInstr.c ./assembler/instructions/sdt.c
emulate: emulate.c ./emulator/memory.c ./emulator/machine.c ./emulator/utilities.c ./emulator/instructions/dataProcessingImm.c ./emulator/instructions/dataProcessingReg.c ./emulator/instructions/sdt.c ./emulator/instructions/branchInstr.c

flappyBird: ./extension/flappyBird/flappyBird.c ./extension/gameEngine.c ./extension/flappyBird/Pipes.c ./extension/flappyBird/Bird.c ./extension/flappyBird/text.c
	$(CC) $(CFLAGS) -o flappyBird ./extension/flappyBird/flappyBird.c ./extension/gameEngine.c ./extension/flappyBird/Pipes.c ./extension/flappyBird/Bird.c ./extension/flappyBird/text.c

pacman: ./extension/pacman/Pacman.c ./extension/gameEngine.c ./extension/pacman/character.c ./extension/pacman/walls.c ./extension/pacman/alien.c ./extension/pacman/stars.c ./extension/flappyBird/text.c
	$(CC) $(CFLAGS) -o pacman ./extension/pacman/Pacman.c ./extension/gameEngine.c ./extension/pacman/character.c ./extension/pacman/walls.c ./extension/pacman/alien.c ./extension/pacman/stars.c ./extension/flappyBird/text.c

pong: ./extension/pong/pong.c ./extension/gameEngine.c ./extension/pong/player.c ./extension/pong/ball.c
	$(CC) $(CFLAGS) -o pong ./extension/pong/pong.c ./extension/gameEngine.c ./extension/pong/player.c ./extension/pong/ball.c

gameOfLife: ./extension/gameOfLife/gameOfLife.c ./extension/gameEngine.c
	$(CC) $(CFLAGS) -o gameOfLife ./extension/gameOfLife/gameOfLife.c ./extension/gameEngine.c

clean:
	$(RM) *.o assemble emulate flappyBird pong gameOfLife pacman
	
