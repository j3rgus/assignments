CC=gcc
CFLAGS=-Wall -Wextra

all: libdebug libpblog

libdebug: libdebug.o
		$(CC) -shared -Wl,-soname,$@.so.1 -o $@.so $<
		ln -s libs/$@.so ../$@.so.1
		rm $<

libdebug.o: libdebug.c
		$(CC) $(CFLAGS) -fPIC -c $<

libpblog: libpblog.o
		$(CC) -shared -Wl,-soname,$@.so.1 -o $@.so $<
		ln -s libs/$@.so ../$@.so.1
		rm $<

libpblog.o: libpblog.c
		$(CC) $(CFLAGS) -fPIC -c $<

clean:
		rm ../libdebug.so.1 ../libpblog.so.1 libdebug.so libpblog.so
