CC=gcc
CFLAGS=-Wall -Wextra -g -I../libs
LDFLAGS=-L../libs
LIBS=-ldebug -lpblog

all: writepb

writepb: writepb.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o ../$@ $< $(LIBS)

clean:
		rm ../writepb
