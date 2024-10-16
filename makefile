CC=gcc
CFLAGS=-c -Wall -Wpedantic
LDFLAGS= -lraylib -lm -lpthread -ldl -lrt -lX11
files:=hello raylib
all:$(files) 


raylib.o: raylib.c
	$(CC) $(LDFLAGS) $(CFLAGS) $^
raylib: raylib.o
	$(CC) $^ $(LDFLAGS) -o $@



hello: hello.o
	$(CC) $^ $(LDFLAGS) -o $@
hello.o: hello.c
	$(CC) $(CFLAGS) $^

clean: 
	rm -f *.o $(files)

