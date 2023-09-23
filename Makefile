CC=gcc
CFLAGS=-g
BINS=word_test lexer_test

all: $(BINS)
word_test: word.o word_test.o
lexer_test: lexer_test.o lexer.o word.o buf.o

style:
	astyle --style=kr *.c *.h

clean:
	rm -f $(BINS) *.o
