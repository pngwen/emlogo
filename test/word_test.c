#include <stdio.h>
#include <string.h>
#include "word.h"

void print_word(const char *msg, struct eml_word *w);
struct eml_word *get_word(const char *msg);

int main()
{
    struct eml_word *w1, *w2, *w3;
    char buf[100];

    /* Read two words */
    w1 = get_word("Word 1: ");
    w2 = get_word("Word 2: ");

    /* Concatenate them */
    w3 = eml_wcat(w1, w2);

    /* print them */
    print_word("w1", w1);
    print_word("w2", w2);
    print_word("w3", w3);

    /* destroy them */
    eml_free_word(w1);
    eml_free_word(w2);
    eml_free_word(w3);


    /* test other creation modes */
    w1 = eml_itow(42);
    w2 = eml_dtow(12.5);
    print_word("itow", w1);
    print_word("dtow", w2);
}


void print_word(const char *msg, struct eml_word *w)
{
    printf("%s Type: %d Str: %s\n", msg, w->type, eml_word_str(w));
}


struct eml_word *get_word(const char *msg)
{
    char buf[100];
    int i;

    printf("%s", msg);
    fgets(buf, 100, stdin);
    i = strlen(buf)-1;
    buf[i] = '\0';
    return eml_stow(buf);
}