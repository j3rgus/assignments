CC=g++
CFLAGS=-Wall -std=c++11
LIBS=-lGL -lglut -lGLU -lGLEW -lIL -lILU
TARGET=main

SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
		$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
		@- rm $^

%.o: %.cpp
		$(CC) -c $(CFLAGS) $< -o $@

.PHONY: clean

clean:
		@- rm $(TARGET) 2> /dev/null
