CC=gcc
CFLAGS=-Wall -Werror -g

all: env

env: printenv.c
		$(CC) $(CFLAGS) printenv.c -o printenv

clean:
		rm -rf *.o printenv
