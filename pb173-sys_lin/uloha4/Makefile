CC=gcc
CFLAGS=-Wall -Wextra -g

all: atexit fork id ps

atexit: atexit.c
		$(CC) $(CFLAGS) atexit.c -o atexit

fork: fork.c
		$(CC) $(CFLAGS) fork.c -o fork

id: id.c
		$(CC) $(CFLAGS) id.c -o id

ps: ps.c
		$(CC) $(CFLAGS) ps.c -o ps

clean:
		rm atexit fork id ps
