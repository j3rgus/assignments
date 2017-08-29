CC=gcc
CFLAGS=-Wall -Wextra -g

_SRC=$(wildcard *.c)
TARGET=$(patsubst %.c,%.so,$(_SRC))

all: $(TARGET)

%.so: %.o
		$(CC) -shared -Wl,-soname,$@.1 -o $@ $<
		@- ln -s libs/$@ ../$@.1
		@- rm $<

%.o: %.c
		$(CC) $(CFLAGS) -c -fPIC $<

.PHONY: clean

clean:
		@- rm ../*.so.1 *.so
