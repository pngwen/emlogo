/*
 * File: list.c
 * Purpose: This is the implementation file for a linked list data structure.
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
#include "list.h"

/************  Static Helper Functions ************************/
static struct eml_list_node *alloc_node()
{
    struct eml_list_node *node;
    node = malloc(sizeof(struct eml_list_node));
    node->next = NULL;

    return node;
}


void destroy_node(struct eml_list *l, struct eml_list_node *prev, struct eml_list_node *cur)
{
    /* nothing to do delete */
    if(!cur) {
        return;
    }

    /* various unlinking cases */
    if(!prev) {
        /* handle deletion of the head */
        l->head = l->head->next;
        if(!l->head) {
            l->tail = NULL;
        }
    } else if(cur == l->tail) {
        /* handle the deletion of the tail */
        l->tail = prev;
        l->tail->next = NULL;
    } else {
        /* handle the deletion of an interior node */
        prev->next = cur->next;
    }

    /* destroy the node */
    free(cur);
}


/********************  List Functions ************************/
/* create a list */
struct eml_list *eml_list_alloc()
{
    struct eml_list *l = malloc(sizeof(struct eml_list));

    l->head = NULL;
    l->tail = NULL;

    return l;
}


/* Destroy a list. Does nothing to ptr data. */
void eml_list_free(struct eml_list *l)
{
    struct eml_list_node *next, *cur;

    /* go through each node and destroy them */
    cur = l->head;
    while(cur) {
        next = cur->next;
        free(cur);
        cur = next;
    }
}


/* append to a list */
void eml_list_append(struct eml_list *l, void *data)
{
    /* create the node */
    struct eml_list_node *node = alloc_node();
    node->data = data;
    node->next = NULL;

    if(!l->tail) {
        /* empty list! */
        l->head = l->tail = node;
    } else {
        /* add it to the end */
        l->tail->next = node;
        l->tail = node;
    }
}


/* insert an item at the head of a list */
void eml_list_push(struct eml_list *l, void *data)
{
    /* create the node */
    struct eml_list_node *node = alloc_node();
    node->data = data;
    node->next = l->head;

    /* link it to the list */
    l->head = node;
    if(!l->tail) {
        l->tail = node;
    }
}


/* insert an item after the given node */
void eml_list_insert(struct eml_list *l, struct eml_list_node *node, void *data)
{
    struct eml_list_node *new_node;

    /* create the node and populate the pointers */
    new_node = alloc_node();
    new_node->data = data;
    new_node->next = node->next;

    /* Is this the new tail? */
    if(node == l->tail) {
        l->tail = new_node;
    }
}


/* apply the given function to the data in each node */
void eml_list_apply(struct eml_list *l, eml_list_visitor visit)
{
    struct eml_list_node *cur;

    /* apply the visit function to each item in the list */
    for(cur = l->head; cur; cur = cur->next) {
        visit(cur->data);
    }
}


/* remove an item from the list */
void eml_list_remove(struct eml_list *l, void *data)
{
    struct eml_list_node *cur, *prev;

    /* find the node and its predecessor */
    prev = NULL;

    /* search for the node */
    for(cur = l->head; cur; cur = cur->next) {
        if(cur->data == data) {
            break;
        }
        prev = cur;
    }

    destroy_node(l, prev, cur);
}


/* remove the indexth node from the list  */
void eml_list_remove_index(struct eml_list *l, int i)
{
    struct eml_list_node *prev, *cur;

    /* find the node and the previous one */
    for(cur = l->head; i && cur; cur=cur->next, i--) {
        prev = cur;
    }

    destroy_node(l, prev, cur);
}