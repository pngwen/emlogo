/*
 * File: list.h
 * Purpose: This is the header file for a linked list data structure.
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
#ifndef LIST_H
#define LIST_H
#include "word.h"

struct eml_list_node {
    void *data;
    struct eml_list_node *next;
};

struct eml_list {
    struct eml_list_node *head;
    struct eml_list_node *tail;
};

typedef void (*eml_list_visitor)(void*);

/* create a list */
struct eml_list *eml_list_alloc();

/* Destroy a list. Does nothing to ptr data. */
void eml_list_free(struct eml_list *l);

/* append to a list */
void eml_list_append(struct eml_list *l, void *data);

/* insert an item at the head of a list */
void eml_list_push(struct eml_list *l, void *data);

/* insert an item after the given node */
void eml_list_insert(struct eml_list *l, struct eml_list_node *node, void *data);

/* apply the given function to the data in each node */
void eml_list_apply(struct eml_list *l, eml_list_visitor visit);

/* remove an item from the list */
void eml_list_remove(struct eml_list *l, void *data);

/* remove the indexth node from the list  */
void eml_list_remove_index(struct eml_list *l, int i);
#endif