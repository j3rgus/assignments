CC=gcc
CFLAGS=-Wall -Werror
LFLAGS=-L.
LIBS=-ldebug

all: libdebug main

libdebug: libdebug.o
		$(CC) -shared -Wl,-soname,$@.so.1 -o $@.so $<
		rm $<

libdebug.o: libdebug.c
		$(CC) $(CFLAGS) -fPIC -c $<

main: main.c
		$(CC) $(CFLAGS) $(LFLAGS) $< -o $@ $(LIBS)

clean:
		rm main libdebug.so
