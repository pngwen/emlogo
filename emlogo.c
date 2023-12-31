/*
 * File: emlogo.c
 * Purpose: The main REPL interface for the emlogo language.
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
#include <stdlib.h>
#include "emlogo.h"
#include "buf.h"

/* global variables */
char *buf; /* buffer for input */
int buf_i;        /* buffer position */
struct eml_lexer *lex;


/* retrieve the next character from the buffer */
static int buf_getchar(); 

/* read a line into our buffer */
static void eml_repl_readline();

/* process a line of input */
static struct eml_node* eml_repl_process_line(int level);

/* allocate a node */
struct eml_node* eml_node_alloc();

/* destroy a node and the thing it points to */
void eml_node_free(struct eml_node *node);

/* print a node */
void eml_node_print(struct eml_node *node);



int main()
{
    struct eml_node *prog_node;

    /* initialize the input buffer and lexer */
    buf = eml_buf_alloc();
    lex = eml_alloc_lexer(buf_getchar);

    while(!feof(stdin)) {
        prog_node = eml_repl_process_line(0);
        eml_node_print(prog_node);
        printf("\n");
        eml_node_free(prog_node);
    }

    eml_free_lexer(lex); 
    eml_buf_free(buf);
}


/* retrieve the next character from the buffer */
static int buf_getchar() 
{
    if(buf_i >= eml_buf_length(buf)) {
        return EOF;
    }

    return buf[buf_i++];
}


/* read a line into our buffer */
static void eml_repl_readline()
{
    int ic;
    char c;

    /* start with an empty buffer */
    eml_buf_clear(buf);
    buf_i = 0;
    while((ic=getchar()) != EOF && ic != '\n') {
        c = (char) ic;
        buf = eml_buf_nappend(buf, &c, 1);
    }
    if(ic != EOF) {
        c = (char) ic;
        buf = eml_buf_nappend(buf, &c, 1);
    }

    /* put in the null terminator */
    buf = eml_buf_nappend(buf, "\0", 1);

    /* reset the lexer */
    lex->cur = 0;
}


/* process a line of input */
static struct eml_node* eml_repl_process_line(int level)
{
    struct eml_list *list;
    struct eml_node *node;
    struct eml_word *word;
    int done = 0;

    /* create the lexer and the list */
    if(!level) eml_repl_readline();
    list = eml_list_alloc();
    
    /* process the line */
    while(!done) {
        word = eml_lexer_next(lex);
        if(!word) {
            if(!level) {
                done = 1;
                continue;
            }
            printf("> ");
            if(feof(stdin)) {
                done = 1;
                continue;
            }
            eml_repl_readline();
            continue;
        }

        if(word->type == TOKEN) {
            if(word->field.s[0] == '[') {
                node = eml_repl_process_line(level+1);
            } else if(word->field.s[0] == ']') {
                /* TODO: Handle error on unexpected ] */
                done = 1;
                continue;
            }
        } else {
            node = eml_node_alloc();
            node->type = EML_WORD;
            node->data = word;
        }
        eml_list_append(list, node);
    }

    node = eml_node_alloc();
    node->type = EML_LIST;
    node->data = list;
    return node;
}



/* allocate a node */
struct eml_node* eml_node_alloc()
{
    return calloc(1, sizeof(struct eml_node));
}


/* destroy a node and the thing it points to */
void eml_node_free(struct eml_node *node)
{
    if(node->type == EML_WORD) {
        /* handle the freeing of the word */
        eml_free_word(node->data);
    } else if(node->type == EML_LIST) {
        /* free the list of nodes */
        eml_list_apply(node->data, (eml_list_visitor)eml_node_free);
        eml_list_free(node->data);
    }
    free(node);
}


/* print a node */
void eml_node_print(struct eml_node *node)
{
    if(node->type == EML_WORD) {
        printf("%s", eml_word_str(node->data));
        putchar(' ');
    } else {
        printf("%s", "[ ");
        eml_list_apply(node->data, (eml_list_visitor)eml_node_print);
        printf("%s", "] ");
    }
}
