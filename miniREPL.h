#ifndef MINIREPL_H
#define MINIREPL_H

#define MINIREPL_DEF static inline
#define DEFAULT_N_TOKENS 100

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef enum token_type {
    NUM,
    ADD,
    SUB,
    MUL,
    DIV,
    LEFT_PAR,
    RIGHT_PAR,

} token_type_e;

typedef struct token {
    token_type_e type;
    const char *text;
    int index;
} token_s;

typedef struct token_list {
    size_t size;
    size_t items;
    token_s *array;
} token_list_s;

MINIREPL_DEF int create_token_list(token_list_s *list);
MINIREPL_DEF int push_token(token_s tok, token_list_s *list);
MINIREPL_DEF int pop_token(token_s *tok, token_list_s *list);
MINIREPL_DEF int get_token(int idx, token_list_s *list, token_s *tok);
    
#define MINIREPL_IMPL
#ifdef MINIREPL_IMPL

MINIREPL_DEF int create_token_list(token_list_s *list) {
    list->size = DEFAULT_N_TOKENS;
    list->items = 0;
    list->array = (token_s *)calloc(DEFAULT_N_TOKENS, sizeof(token_s));
    if (list->array == NULL) {
        return -1;
    }
    return 0;
}

MINIREPL_DEF int push_token(token_s tok, token_list_s *list) {
    if (list->items + 1 > list->size) {
        list->array = (token_s *)reallocarray(list->array, list->size + DEFAULT_N_TOKENS, sizeof(token_s));
        if (list->array == NULL) {
            return -1;
        }
        list->size = list->size * 2;
    }
    list->items += 1;
    list->array[list->items - 1] = tok;
    return 0;
}

MINIREPL_DEF int pop_token(token_s *tok, token_list_s *list) {
    if (list->items <= 0) {
        return -1;
    }
    memcpy(tok, &list->array[list->items - 1], sizeof(token_s));
    memset(&list->array[list->items - 1], 0, sizeof(token_s));
    list->items -= 1;
    return 0;
}

MINIREPL_DEF int get_token(int idx, token_list_s *list, token_s *tok) {
    if (idx < 0 || idx >= list->items) {
        return -1;
    }
    memcpy(tok, &list->array[idx], sizeof(token_s));
    return 0;
}


#endif
#endif
