CC=gcc
CFLAGS=-Wall -Werror -g
SRC=debug.c

all: debug

debug: $(SRC)
		$(CC) $(CFLAGS) $< -o $@

clean:
		rm -rf *.o debug
