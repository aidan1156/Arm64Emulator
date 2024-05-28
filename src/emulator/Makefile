CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

PHONY: all clean

SRC = machine.c memory.c 

all: machine.o memory.o

clean: 
	rm -f machine.o memory.o

%.o: %.c
	%(CC) $(CFLAGS) -c $< -o $@