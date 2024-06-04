CC      ?= gcc
CFLAGS  ?= -std=c17 -g -D_POSIX_SOURCE -D_DEFAULT_SOURCE -Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

<<<<<<< HEAD
SRC =  ../fileIO.c ../maps.c ../utilities.c ../../emulator/utilities.c sdt.c
=======
SRC =  ../fileIO.c ../maps.c ../utilities.c branchInstr.c
>>>>>>> 3885deef12d33e635b0718fe3bdc86ed90cae5cf
OBJ = $(SRC:.c=.o)
TARGET = sdt

all: $(TARGET)

$(TARGET): $(OBJ)
<<<<<<< HEAD
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJ) $(TARGET)
=======
    $(CC) $(CFLAGS) -o $@ $^

%.o: %.c
    $(CC) $(CFLAGS) -c $< -o $@

clean: 
    rm -f $(OBJ) $(TARGET)
>>>>>>> 3885deef12d33e635b0718fe3bdc86ed90cae5cf
