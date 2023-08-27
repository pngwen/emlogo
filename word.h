/*
 * File: word.h
 * Purpose: This is the header file for the emlogo word storage type.
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
#ifndef WORD_H
#define WORD_H

/* tokens and word types */
extern const char *EML_TOKENS;
enum eml_word_type { WORD = 0, INTEGER, FLOAT, TOKEN };

union eml_word_field {
  char *s;  /* string word */
  int i;    /* integer word */
  double d; /* floating point word */
};

struct eml_word {
  union eml_word_field field;
  enum eml_word_type type;
};

/* word creation functions */
struct eml_word *eml_stow(char *s);  /* string to word */
struct eml_word *eml_itow(int i);    /* integer to word */
struct eml_word *eml_dtow(double d); /* double to word */

/* Concatenate two words */
struct eml_word *eml_wcat(struct eml_word *a, struct eml_word *b);

/* Convert word to string */
char *eml_word_str(struct eml_word *w);

/* word destructor */
void eml_free_word(struct eml_word *w);

#endif
