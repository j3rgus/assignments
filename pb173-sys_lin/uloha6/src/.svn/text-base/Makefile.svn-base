CC=gcc
CFLAGS=-Wall -Wextra -g -I../libs
LDFLAGS=-L../libs
LIBS=-pthread -ldebug

_SRC=$(wildcard *.c)
TARGET=$(patsubst %.c,../%,$(_SRC))

all: $(TARGET)

$(TARGET): ../%: %.o
		$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LIBS)
		@- rm $<

%.o: %.c
		$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
		@- rm $(TARGET)
