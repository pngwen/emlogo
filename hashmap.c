/*
 * File: hashmap.c
 * Purpose: This is the implementation file for a hashmap which maps words to void pointers.
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
#include "hashmap.h"
#define EML_HASHMAP_INIT_CAP 256

/* Helper function to allocate buckets and initialize them to null */
struct eml_hashmap_bucket *eml_alloc_bucket(int n)
{
    return calloc(n, sizeof(struct eml_hashmap_bucket));
}

/* create a hashmap */
struct eml_hashmap *eml_hashmap_alloc();

/* sets an item in the hashmap */
void eml_hashmap_set(struct eml_hashmap *map, struct eml_word *word, void *data);

/* retrieves an item from the hashmap */
void *eml_hashmap_get(struct eml_hashmap *map, struct eml_word *word);