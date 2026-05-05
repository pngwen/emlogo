/*
 * File: buf.c
 * Purpose: This is the implementation file for a dynamic buffer structure.
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
#include <stdlib.h>
#include <string.h>
#include "buf.h"

#define INIT_CAPACITY 128
#define BUF_INFO(p) (((struct buf_info*) (p))-1)
#define BUF_ALLOC(n) malloc(sizeof(struct buf_info) + (n))

struct buf_info {
    unsigned int capacity;
    unsigned int length;
};

/* static reallocation function */
static char *eml_buf_grow(char *buf, unsigned int nsize)
{
    int ncap = BUF_INFO(buf)->capacity;
    char *nbuf;

    /* detect the nothing to do case */
    if(nsize <= ncap) {
        return buf;
    }

    /* figure out the new capacity by doubling */
    while(ncap < nsize) {
        ncap *= 2;
    }

    /* allocate the new buffer */
    nbuf = BUF_ALLOC(ncap);
    BUF_INFO(nbuf)->capacity = ncap;
    BUF_INFO(nbuf)->length = BUF_INFO(buf)->length;

    /* copy the old and clean up */
    memcpy(nbuf, buf, BUF_INFO(buf)->length);
    eml_buf_free(buf);

    return nbuf;
}

/* Create a new character buffer. */
char *eml_buf_alloc()
{
    char *buf;

    /* allocate enough room to store the buffer and info struct */
    buf = BUF_ALLOC(INIT_CAPACITY);
    if(!buf) {
        return buf;
    }

    /* point to the right part of the buffer */
    buf += sizeof(struct buf_info);

    /* populate the info */
    BUF_INFO(buf)->capacity = INIT_CAPACITY;
    BUF_INFO(buf)->length = 0;

    return buf;
}


/* Destroy a character buffer. */
void eml_buf_free(char *buf)
{
    /* free it up */
    free(BUF_INFO(buf));
}


/* Append n bytes to the buffer. Note! This may result in reallocation, so it should
   be called like this:
       buf = eml_buf_nappend(buf, bytes, n);
 */
char *eml_buf_nappend(char *buf, char *bytes, int n)
{
    /* make enough room */
    buf = eml_buf_grow(buf, BUF_INFO(buf)->length + n);

    /* perform the copy */
    memcpy(buf + BUF_INFO(buf)->length, bytes, n);
    BUF_INFO(buf)->length += n;

    return buf;
}


/* Append the null terminated string onto the buffer. This may result in reallocation, so
   it should be called like this:
      buf = eml_buf_apend(char *buf, char *str);
   This function copies the null terminator. (which is included in the length!)
 */
char *eml_buf_append(char *buf, char *str)
{
    return eml_buf_nappend(buf, str, strlen(str)+1);
}


/* Get the length of the buffer */
int eml_buf_length(char *buf)
{
    return BUF_INFO(buf)->length;
}


/* Get the capacity of the buffer */
int eml_buf_capacity(char *buf)
{
    return BUF_INFO(buf)->capacity;
}


/* Clear the buffer */
void eml_buf_clear(char *buf)
{
    BUF_INFO(buf)->length = 0;
}
