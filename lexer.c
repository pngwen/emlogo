/*
 * File: lexer.c
 * Purpose: This is the implementatiom  file for the emlogo lexer.
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
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "buf.h"

/* constants */
const char* STOP_SYM = "[]";


/* helper function prototypes */
static void next_char(struct eml_lexer *lex);
static void skip_to_word(struct eml_lexer *lex);
static void set_text(struct eml_lexer *lex, struct eml_word *word);
static void consume(struct eml_lexer *lex);
static int stop_word(struct eml_lexer *lex);


/* Create a lexer for the given file stream */
struct eml_lexer* eml_alloc_lexer(eml_getchar getchar)
{
    /* create the basic structure */
    struct eml_lexer *lex = malloc(sizeof(struct eml_lexer));

    /* initialize the fields */
    lex->line = 1;
    lex->buf = eml_buf_alloc();
    lex->col = 0;
    lex->cur = 0;
    lex->getchar = getchar;

    return lex;
}


/* deallocate the lexer */
void free_lexer(struct eml_lexer *lex)
{
    /* deallocate the dynamic fields */
    eml_buf_free(lex->buf);

    /* finish the job */
    free(lex);
}


/* get the next word from the lexer, returns null on end of inpit */
struct eml_word* eml_lexer_next(struct eml_lexer *lex)
{
    int done;

    /* get ready for the next word */
    eml_buf_clear(lex->buf);

    /* get the start of the word */
    skip_to_word(lex);
    if(lex->cur == EOF) {return NULL;}
    done=stop_word(lex);

    /* scan the string */
    do {
        consume(lex);
        if(!done) {
            done=stop_word(lex);
        }
    } while(!done);

    /* null terminate the string */
    eml_buf_append(lex->buf, "");

    return eml_stow(lex->buf);
}


/******************************************
 * Helper functions
 ******************************************/
static void next_char(struct eml_lexer *lex)
{
    int nc = lex->getchar();

    /* translate null character */
    if(nc==0) {
        nc = EOF;
    }

    /* detect end of file or update line and col */
    if(nc == '\n') {
        lex->line++;
        lex->col++;
    } else if(lex->cur == '\n') {
        lex->col=1;
    } else if(nc != EOF) {
        lex->col++;
    }

    /* set the current character */
    lex->cur = nc;
}


static void skip_to_word(struct eml_lexer *lex)
{
    int done=0;

    do {
        /* stop on the first non-space  */
        if(lex->cur && !isspace(lex->cur)) {
            done=1;
        } else {
            next_char(lex);
        }
    } while(!done);
}


static void consume(struct eml_lexer *lex)
{
    char *buf;  /* buffer for reallocation, if needed. */
    char c = lex->cur;

    /* add the character to the end */
    eml_buf_nappend(lex->buf, &c, 1);
    next_char(lex);
}


static int stop_word(struct eml_lexer *lex)
{
    const char *sym = STOP_SYM;

    /* stop on space */
    if(isspace(lex->cur)) {
        return 1;
    }

    /* check for the stop symbols */
    while(*sym) {
        if(*sym == lex->cur) {
            return 1;
        }
        sym++;
    }

    return 0;
}
