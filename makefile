CC=gcc
CFLAGS=-c -Wall -Wpedantic
LDFLAGS=

all: hello

hello: hello.o
	$(CC) hello.o -o hello
hello.o: hello.c
	$(CC) $(LDFLAGS) $(CFLAGS) hello.c

clean: 
	rm -f *.o hello

