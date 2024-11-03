CC=gcc
CFLAGS=-c -Wall -Wpedantic -g
LDFLAGS= -lraylib -lm -lpthread -ldl -lrt -lX11 -g
files:=hello
all:$(files) 

hello: hello.o chip.o keyboard.o
	$(CC) $^ $(LDFLAGS) -o $@

chip.o: chip.c chip.h
	$(CC) $(LDFLAGS) $(CFLAGS) chip.c

keyboard.o: keyboard.c
	$(CC) $(CFLAGS) $^

hello.o: hello.c
	$(CC) $(CFLAGS) $^

clean: 
	rm -f *.o $(files)

