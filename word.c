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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "word.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* constants */
const char *EML_TOKENS = "[]";

/* some buffer space for us to use */
static char buf[200];

/* helper function to allocate words */
static struct eml_word *eml_word_alloc()
{
    return malloc(sizeof(struct eml_word));
}

/* helper function to compute hash for bytes */
static unsigned int byte_hash(void *ptr, int n)
{
    unsigned char *byte;
    unsigned int hash = 0;
    for (byte = ptr; n; n--, byte++) {
        hash += 31 * toupper(*byte);
    }

    return hash;
}

/* word creation functions */
struct eml_word *eml_stow(char *s)
{
    int len;
    struct eml_word *w;
    enum eml_word_type type;
    int i;
    const char *tptr;

    /* initialize things */
    len = strlen(s);

    /* scan the string for numeric type */
    if ((len > 1 && *s == '-') || isdigit(*s)) {
        type = INTEGER;
    }
    if (*s == '.' && len > 1) {
        w->type = FLOAT;
    }

    for (i = 1; type != WORD && i < len; i++) {
        /* digits pass */
        if (isdigit(s[i])) {
            continue;
        }

        /* a float is born */
        if (type == INTEGER && s[i] == '.') {
            type = FLOAT;
            continue;
        }

        /* aaaand, if we make it here, it's just a word */
        type = WORD;
    }

    /* handle the types */
    if (type == INTEGER) {
        w = eml_itow(atoi(s));
    } else if (type == FLOAT) {
        w = eml_dtow(atof(s));
    } else {
        w = eml_word_alloc();
        w->field.s = malloc(len + 1);
        w->type = type;
        strcpy(w->field.s, s);
        w->hash = byte_hash(s, len);

        /* detect tokens */
        if (len == 1) {
            for (tptr = EML_TOKENS; *tptr; tptr++) {
                if (*tptr == *(w->field.s)) {
                    w->type = TOKEN;
                }
            }
        }
    }

    return w;
}

struct eml_word *eml_itow(int i)
{
    struct eml_word *w = eml_word_alloc();
    w->type = INTEGER;
    w->field.i = i;
    w->hash = byte_hash(&(w->field.i), sizeof(int));
    return w;
}

struct eml_word *eml_dtow(double d)
{
    struct eml_word *w = eml_word_alloc();
    w->type = FLOAT;
    w->field.d = d;
    w->hash = byte_hash(&(w->field.d), sizeof(double));
    return w;
}

static char *word_as_str(char *bstart, struct eml_word *w)
{
    /* handle the easy case */
    if (w->type == WORD || w->type == TOKEN) {
        return w->field.s;
    }

    /* handle the harder cases */
    if (w->type == INTEGER) {
        sprintf(bstart, "%d", w->field.i);
    } else if (w->type == FLOAT) {
        sprintf(bstart, "%g", w->field.d);
    }

    return bstart;
}

/* Concatenate two words */
struct eml_word *eml_wcat(struct eml_word *a, struct eml_word *b)
{
    int la, lb;    /* string lengths */
    char *sa, *sb; /* a and b as strings */
    char *s;       /* the string we are building */
    struct eml_word *w;

    /* get the word strings */
    sa = word_as_str(buf, a);
    sb = word_as_str(buf + 100, b);

    /* build the string */
    la = strlen(sa);
    lb = strlen(sb);
    s = malloc(la + lb);
    strncpy(s, sa, la);
    strncpy(s + la, sb, lb + 1);

    /* build the word */
    w = eml_stow(s);
    if (w->type != WORD) {
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
void eml_free_word(struct eml_word *w)
{
    /* free any dynamically allocated character data */
    if (w->type == WORD || w->type == TOKEN) {
        free(w->field.s);
    }

    free(w);
}

/*
 * Returns 1 if w1 = w2, 0 otherwise
 */
int eml_word_equals(struct eml_word *w1, struct eml_word *w2)
{
    /* first compare their easy parts */
    if(w1->type != w2->type || w1->hash != w2->hash) {
        return 0;
    }

    /* compare their fields according to type */
    if(w1->type == INTEGER) {
        return w1->field.i == w2->field.i;
    } else if(w2->type == FLOAT) {
        return w1->field.d == w2->field.d;
    } else {
        return strcasecmp(w1->field.s, w2->field.s) == 0;
    }
}