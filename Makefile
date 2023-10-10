CC=gcc
CFLAGS=-g
BINS=word_test lexer_test emlogo

all: $(BINS)
word_test: word.o word_test.o
lexer_test: lexer_test.o lexer.o word.o buf.o hashmap.o
emlogo: emlogo.o lexer.o word.o buf.o hashmap.o list.o

style:
	astyle --style=1tbs *.c *.h

clean:
	rm -f $(BINS) *.o
