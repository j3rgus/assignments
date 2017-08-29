CC=gcc
CFLAGS=-Wall -Werror -g
SRC=unit.c
LIBS=-lcheck

all: unit

unit: $(SRC)
		$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
		rm -rf *.o unit
