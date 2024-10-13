CC=gcc
CFLAGS=-c -Wall -Wpedantic
LDFLAGS=
RAYLIBFLAGS= -lraylib -lm -lpthread -ldl -lrt -lX11
files:=hello raylib
all:$(files) 


raylib.o: raylib.c
	$(CC) $(LDFLAGS) $(CFLAGS) $^
raylib: raylib.o
	$(CC) $(LDFLAGS) $^ $(RAYLIBFLAGS) -o $@



hello: hello.o
	$(CC) $^ -o $@
hello.o: hello.c
	$(CC) $(LDFLAGS) $(CFLAGS) $^

clean: 
	rm -f *.o $(files)

