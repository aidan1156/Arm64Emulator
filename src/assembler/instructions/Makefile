CC      ?= gcc
CFLAGS  ?= -std=c17 -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

SRC =  ../fileIO.c ../maps.c ../utilities.c sdt.c
OBJ = $(SRC:.c=.o)
TARGET = sdt

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) $(TARGET)
