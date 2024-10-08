CC=gcc
CFLAGS=-c -Wall
LDFLAGS=

all: hello

hello: hello.o
	$(CC) hello.o -o hello
hello.o: hello.c
	$(CC) $(LDFLAGS) $(CFLAGS) hello.c

clean: 
	rm -f *.o hello

