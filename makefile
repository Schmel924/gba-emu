CC=gcc
CFLAGS=-c -Wall -Wpedantic
LDFLAGS= -lraylib -lm -lpthread -ldl -lrt -lX11
files:=hello
all:$(files) 

hello: hello.o chip.o
	$(CC) $^ $(LDFLAGS) -o $@

chip.o: chip.c chip.h
	$(CC) $(LDFLAGS) $(CFLAGS) chip.c



hello.o: hello.c
	$(CC) $(CFLAGS) $^

clean: 
	rm -f *.o $(files)

