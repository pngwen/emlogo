/*
 * File: word.c
 * Purpose: This is the implementation file for the emlogo word storage type.
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
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "word.h"

/* some buffer space for us to use */
static char buf[200];


/* helper function to allocate words */
static struct eml_word *eml_word_alloc()
{
    return malloc(sizeof(struct eml_word));
}


/* word creation functions */
struct eml_word* eml_stow(char *s)
{
    int len;
    struct eml_word *w;
    int i;

    /* initialize things */
    len = strlen(s);
    w = eml_word_alloc();

    /* scan the string for numeric type */
    if((len > 1 && *s == '-') || isdigit(*s)) { w->type = INTEGER; }
    if(*s == '.' && len > 1) { w->type = FLOAT; }
    
    for(i=1; w->type != WORD && i<len; i++) {
        
        /* digits pass */
        if(isdigit(s[i])) { 
            continue; 
        }

        /* a float is born */
        if(w->type == INTEGER && s[i] == '.') {
            w->type = FLOAT;
            continue; 
        }

        /* aaaand, if we make it here, it's just a word */
        w->type = WORD;
    }

    /* handle the types */
    if(w->type == INTEGER) {
        w->field.i = atoi(s);
    } else if(w->type == FLOAT) {
        w->field.d = atof(s);
    } else {
        w->field.s = malloc(len+1);
        strcpy(w->field.s, s);
    }
    
    return w;
}


struct eml_word* eml_itow(int i)
{
    struct eml_word *w = eml_word_alloc();
    w->type = INTEGER;
    w->field.i = i;
    return w;
}


struct eml_word* eml_dtow(double d)
{
    struct eml_word *w = eml_word_alloc();
    w->type = FLOAT;
    w->field.d = d;
    return w;
}


static char *word_as_str(char *bstart, struct eml_word *w)
{
    /* handle the easy case */
    if(w->type == WORD) {
        return w->field.s;
    }

    /* handle the harder cases */
    if(w->type == INTEGER) {
        sprintf(bstart, "%d", w->field.i);
    } else if(w->type == FLOAT) {
        sprintf(bstart, "%g", w->field.d);
    }

    return bstart;
}


/* Concatenate two words */
struct eml_word* eml_wcat(struct eml_word *a, struct eml_word *b)
{
    int la, lb;         /* string lengths */
    char *sa, *sb;      /* a and b as strings */
    char *s;            /* the string we are building */
    struct eml_word *w;

    /* get the word strings */
    sa = word_as_str(buf, a);
    sb = word_as_str(buf+100, b);

    /* build the string */
    la = strlen(sa);
    lb = strlen(sb);
    s = malloc(la + lb);
    strncpy(s, sa, la);
    strncpy(s+la, sb, lb+1);

    /* build the word */
    w = eml_stow(s);
    if(w->type != WORD) {
        free(s);
    }

    return w;
}


/* Convert word to string */
char *eml_word_str(struct eml_word *w)
{
    return word_as_str(buf, w);
}


/* word destructor */
void eml_free_word(struct eml_word* w)
{
    /* free any dynamically allocated character data */
    if(w->type == WORD || w->type == TOKEN) {
        free(w->field.s);
    }

    free(w);
}
