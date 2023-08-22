CC=gcc
CFLAGS=-g
BINS=word_test

all: $(BINS)
word_test: word.o word_test.o

clean:
	rm -f $(BINS) *.o
