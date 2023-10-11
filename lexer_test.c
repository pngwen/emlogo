/*
 * File: lexer_test.c
 * Purpose: A simple test of the emlogo lexer.
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
#include <stdio.h>
#include "lexer.h"
#include "hashmap.h"

int main()
{
    struct eml_lexer *lex;
    struct eml_word *word;
    struct eml_hashmap *h;
    void *count;

    h=eml_hashmap_alloc();

    lex = eml_alloc_lexer(getchar);
    while(word=eml_lexer_next(lex)) {
        count = eml_hashmap_get(h, word);
        if(count) {
            eml_hashmap_set(h, word, count+1);
        } else {
            eml_hashmap_set(h, word, (void*)1);
        }
        count = eml_hashmap_get(h, word);
        printf("Type: %d Hash: %u Text: %s, Count: %d\n", 
                word->type, word->hash, eml_word_str(word), count);
    } 
}
