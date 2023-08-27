/*
 * File: buf.h
 * Purpose: This is the header file for a dynamic buffer structure.
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
#ifndef BUF_H
#define BUF_H

/* This dynamic buffer will operate like a char * for most purposes. It does 
   indeed point to the first byte of the buffer. They are not necessarily null 
   terminated unless you use the append method with null terminated strings 
   or you manually add the terminator. 

   It should be noted that these are not, in fact, just character buffers. 
   The actual memory layout looks like this:

   +---------------+-------------+--------------+
   | Private Info  | Buffer Data | Unused Space | 
   +---------------+-------------+--------------+
                    ^                           ^
                char* buf                   buf + capacity

    This is how reallocations are managed while maintaining interoperability.
*/

/* Create a new character buffer. */
char *eml_buf_alloc();

/* Destroy a character buffer. */
void eml_buf_free(char *buf);

/* Append n bytes to the buffer. Note! This may result in reallocation, so it should
   be called like this:
       buf = eml_buf_nappend(buf, bytes, n);
 */
char *eml_buf_nappend(char *buf, char *bytes, int n);

/* Append the null terminated string onto the buffer. This may result in reallocation, so 
   it should be called like this:
      buf = eml_buf_apend(char *buf, char *str);
 */
char *eml_buf_append(char *buf, char *str);

/* Get the length of the buffer */
int eml_buf_length(char *buf);

/* Get the capacity of the buffer */
int eml_buf_capacity(char *buf);

/* Clear the buffer */
void eml_buf_clear(char *buf);
#endif