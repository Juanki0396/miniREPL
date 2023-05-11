#ifndef MINIREPL_H
#define MINIREPL_H

#define MINIREPL_DEF 
#define DEFAULT_N_TOKENS 100

#include <stddef.h>

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

MINIREPL_DEF int create_token_list(token_list_s **list);
MINIREPL_DEF int push_token(token_s tok, token_list_s *list);
MINIREPL_DEF int pop_token(token_s *tok, token_list_s *list);
MINIREPL_DEF int get_token(size_t idx, token_list_s *list, token_s *tok);
MINIREPL_DEF void delete_token_list(token_list_s **list);

extern token_list_s *tok_list;

#endif
