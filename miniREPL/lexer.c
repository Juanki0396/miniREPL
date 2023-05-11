#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

MINIREPL_DEF int create_token_list(token_list_s **list) {
    *list = (token_list_s *)malloc(sizeof(token_list_s));
    if (list == NULL) {
        return -1;
    }
    (*list)->size = DEFAULT_N_TOKENS;
    (*list)->items = 0;
    (*list)->array = (token_s *)calloc(DEFAULT_N_TOKENS, sizeof(token_s));
    if ((*list)->array == NULL) {
        return -1;
    }
    return 0;
}

MINIREPL_DEF int push_token(token_s tok, token_list_s *list) {
    if (list->items + 1 > list->size) {
        printf("List to small. Making it bigger\n");
        size_t new_size = list->size + DEFAULT_N_TOKENS;
        list->array = (token_s *)realloc(list->array, sizeof(token_s) * new_size);
        if (list->array == NULL) {
            return -1;
        }
        list->size = new_size;
        memset(&list->array[list->items], 0, sizeof(token_s) * DEFAULT_N_TOKENS);
    }
    list->items += 1;
    printf("Inserting token\n");
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

MINIREPL_DEF int get_token(size_t idx, token_list_s *list, token_s *tok) {
    if (idx >= list->items) {
        return -1;
    }
    memcpy(tok, &list->array[idx], sizeof(token_s));
    return 0;
}

MINIREPL_DEF void delete_token_list(token_list_s **list) {
    free((*list)->array);
    (*list)->array = 0;
    free(*list);
    *list = 0;
}

token_list_s *tok_list = 0;
