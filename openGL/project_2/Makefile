CC=g++
CFLAGS=-Wall -Werror
SRC=main.cpp ObjLoader.cpp
LIBS=-lGL -lglut -lGLU -lIL -lILU

all: main

main: $(SRC)
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

clean:
		rm main
