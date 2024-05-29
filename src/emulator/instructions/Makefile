CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

PHONY: all clean

SRC = dataprocessingImm.c dataprocessingReg.c

all: dataprocessingImm.o dataprocessingReg.o

clean: 
	rm -f dataprocessingImm.o dataprocessingReg.o

%.o: %.c
	%(CC) $(CFLAGS) -c $< -o $@
