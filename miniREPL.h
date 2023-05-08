#ifndef MINIREPL_H
#define MINIREPL_H

#define MINIREPL_DEF static inline
#define DEFAULT_N_TOKENS 100

#include <stddef.h>
#include <stdlib.h>

enum token_type_e {
    NUM,
    ADD,
    NEG,
    MUL,
    DIV
};

typedef struct token_s {
    token_type_e type;
    const char *text;
    int index;
} token;

typedef struct token_list_s {
    size_t size;
    size_t items;
    token *array;
} token_list;

MINIREPL_DEF int create_token_list(token_list *list);
MINIREPL_DEF int push_token(token tok, token_list *list);
MINIREPL_DEF int pop_token(token tok, token_list *list);
MINIREPL_DEF int get_token(int idx, token_list *list, token *tok);
    
#define MINIREPL_IMPL
#ifdef MINIREPL_IMPL

MINIREPL_DEF int create_token_list(token_list *list) {
    list->size = DEFAULT_N_TOKENS;
    list->items = 0;
    list->array = (token *)calloc(DEFAULT_N_TOKENS, sizeof(token));
    if (list->array == NULL) {
        return -1;
    }
    return 0;
}

#endif
#endif
