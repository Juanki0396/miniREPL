#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MINIREPL_IMP
#include "miniREPL.h"


int main() {

    printf("Creating a list\n");
    token_list_s *list = 0;
    assert(!create_token_list(&list));
    printf("List pointing to %p\n", (void *)list);

    printf("Creating a token\n");
    token_s tok = { .type = SUB, .text = "-", .index = 4 };
    assert(!push_token(tok, list));
    assert(list->items == 1);
    assert(!push_token(tok, list));
    assert(list->items == 2);

    printf("Getting a token from list\n");
    token_s tok2;
    assert(!get_token(1, list, &tok2));
    assert(tok2.index == tok.index);
    assert(tok2.type == tok.type);
    assert(!strcmp(tok2.text, tok.text));

    printf("Deleting a token from list\n");
    token_s tok3;
    assert(!pop_token(&tok3, list));
    assert(tok3.index == tok.index);
    assert(tok3.type == tok.type);
    assert(!strcmp(tok3.text, tok.text));

    printf("Getting a token from list\n");
    assert(!get_token(0, list, &tok2));
    assert(tok2.index == tok.index);
    assert(tok2.type == tok.type);
    assert(!strcmp(tok2.text, tok.text));

    printf("Deleting the list\n");
    delete_token_list(&list);
    assert(list == 0);
    return EXIT_SUCCESS;
}
