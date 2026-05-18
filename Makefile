CC=gcc
CFLAGS=-Wall -g

all: processor

processor: src/processor.c
	$(CC) $(CFLAGS) -o processor src/processor.c

clean:
	rm -f processor