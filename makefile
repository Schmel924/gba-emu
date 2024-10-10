CC=gcc
CFLAGS=-c -Wall -Wpedantic
LDFLAGS=
RAYLIBFLAGS= -lraylib -lm -lpthread -ldl -lrt -lX11
all: hello raylib


raylib.o: raylib.c
	$(CC) $(LDFLAGS) $(CFLAGS)  raylib.c
raylib: raylib.o
	$(CC) $(LDFLAGS) raylib.o $(RAYLIBFLAGS) -o raylib



hello: hello.o
	$(CC) hello.o -o hello
hello.o: hello.c
	$(CC) $(LDFLAGS) $(CFLAGS) hello.c

clean: 
	rm -f *.o hello raylib

