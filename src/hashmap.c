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
#define EML_HASHMAP_LOADFACTOR 80

/* Helper function to allocate buckets and initialize them to null */
static struct eml_hashmap_bucket *eml_alloc_bucket(int n)
{
    return calloc(n, sizeof(struct eml_hashmap_bucket));
}


/* helper function to set up the hashmap with the given number of
 * buckets. It leaves the size undisturbed.
 */
static void eml_hashmap_setup(struct eml_hashmap *h, int n) 
{
    h->bucket = calloc(n, sizeof(struct eml_hashmap_bucket));
    h->cap = n;
    h->limit = (h->cap * EML_HASHMAP_LOADFACTOR) / 100;
}


/* Helper function to rehash the map, doubling the number of buckets */
static void eml_hashmap_rehash(struct eml_hashmap *h)
{
    struct eml_hashmap_bucket *obucket;
    int ocap;
    int i;

    /* get the old information from the table */
    obucket = h->bucket;
    ocap = h->cap;

    /* create the the new table */
    eml_hashmap_setup(h, ocap * 2);

    /* insert all the items from the old */
    for(i=0; i<ocap; i++) {
        if(obucket[i].word) {
            eml_hashmap_set(h, obucket[i].word, obucket[i].data);
        }
    }

    /* destroy the old bucket list */
    free(obucket);
}


/* find the bucket fornthe given word */
static int eml_hashmap_probe(struct eml_hashmap *map, struct eml_word *word) 
{
    int key;
    int pd=1;

    /* get the initial key */
    key = word->hash % map->cap;

    /* probe as needed */
    while(map->bucket[key].word && ! eml_word_equals(map->bucket[key].word, word)) {
        key = word->hash + (pd + pd*pd)/2;
        key %= map->cap;

        pd++;
    }

    return key;
}


/* create a hashmap */
struct eml_hashmap *eml_hashmap_alloc()
{
    struct eml_hashmap *h;

    /* create the initial hashmap */
    h = malloc(sizeof(struct eml_hashmap));
    eml_hashmap_setup(h, EML_HASHMAP_INIT_CAP);

    return h;
}


/* sets an item in the hashmap */
void eml_hashmap_set(struct eml_hashmap *map, struct eml_word *word, void *data)
{
    int key = eml_hashmap_probe(map, word);

    /* set the data */
    map->bucket[key].data = data;

    /* handle growth */
    if(!map->bucket[key].word) {
        map->bucket[key].word = word;
        map->size++;
        if(map->size >= map->limit) {
           eml_hashmap_rehash(map); 
        }
    }
}


/* retrieves an item from the hashmap */
void *eml_hashmap_get(struct eml_hashmap *map, struct eml_word *word)
{
    int key = eml_hashmap_probe(map, word);
    return map->bucket[key].data;
}
