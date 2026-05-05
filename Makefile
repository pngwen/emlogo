CC=gcc
CFLAGS=-g -I include
BINS=word_test lexer_test emlogo
S=src
T=test

all: $(BINS)
word_test: $S/word.o $T/word_test.o
	gcc $(CFLAGS) -o $@ $^
lexer_test: $T/lexer_test.o $S/lexer.o $S/word.o $S/buf.o $S/hashmap.o
	gcc $(CFLAGS) -o $@ $^
emlogo: $S/emlogo.o $S/lexer.o $S/word.o $S/buf.o $S/hashmap.o $S/list.o
	gcc $(CFLAGS) -o $@ $^

style:
	astyle --style=1tbs *.c *.h

clean:
	rm -f $(BINS) $S/*.o $T/*.o
