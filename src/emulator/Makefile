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
