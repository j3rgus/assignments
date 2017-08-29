CC=gcc
CFLAGS=-Wall -Wextra -g -I../libs
LDFLAGS=-L../libs
LIBS=-ldebug

all: pblogd mfifo pipe signal

pblogd: pblogd.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o ../$@ $< $(LIBS)

mfifo: mfifo.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o ../$@ $< $(LIBS)

pipe: pipe.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o ../$@ $< $(LIBS)

signal: signal.c
		$(CC) $(CFLAGS) $(LDFLAGS) -o ../$@ $< $(LIBS)

clean:
		rm ../pblogd ../mfifo ../pipe ../signal
