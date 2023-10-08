/*
 * File: lexer.h
 * Purpose: This is the header file for the emlogo lexer.
 *
 * MIT License
 *
 * Copyright (c) 2023 Robert Lowe
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef LEXER_H
#define LEXER_H
#include <stdio.h>
#include "word.h"

/* lexer state */
struct eml_lexer {
    FILE *file;        /* the stream we are reading */
    int line;          /* line number we are reading */
    int col;           /* column number we are readin*/
    char *buf;         /* The temporary buffer for the lexeme */
    char cur;          /* current character */
};


/* Create a lexer for the given file stream */
struct eml_lexer* eml_alloc_lexer(FILE *file);

/* deallocate the lexer */
void eml_free_lexer(struct eml_lexer *lex);

/* get the next word from the lexer, returns null on end of inpit */
struct eml_word* eml_lexer_next(struct eml_lexer *lex);
#endif
