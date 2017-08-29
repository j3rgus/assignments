CC=gcc

all: dload hlib blib

dload: dload.c
		$(CC) -Wall -o dload dload.c -ldl

hlib: hellolib.o
		$(CC) -shared -Wl,-soname,hello.so.1 -o hello.so.1.0.0 hellolib.o
		rm hellolib.o

hellolib.o: hellolib.c
		$(CC) -fPIC -c hellolib.c

blib: byelib.o
		$(CC) -shared -Wl,-soname,bye.so.1 -o bye.so.1.0.0 byelib.o
		rm byelib.o

byelib.o: byelib.c
		$(CC) -fPIC -c byelib.c

clean:
		rm -rf *.o *.0 dload
