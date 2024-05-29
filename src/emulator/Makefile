<<<<<<< HEAD
CC = gcc
CFLAGS = -Wall -Wextra -g
SRC = machine.c sdt.c
OBJ = $(SRC:.c=.o)
TARGET = sdt

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(OBJ) $(TARGET)
=======
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
>>>>>>> setting-up-cpu
