#ifndef MINIREPL_H
#define MINIREPL_H

#define MINIREPL_DEF 
#define DEFAULT_N_TOKENS 100

#include <stddef.h>

typedef enum token_type {
    // 1 char tokens
    ADD, SUB, MUL, DIV, MOD, ASSIGN, NEG, MORE, LESS, LEFT_PAR, RIGHT_PAR,
    LEFT_BRA, RIGHT_BRA, END_STATEMENT,

    // 2 char tokens
    ADDEQ, SUBEQ, MULEQ, DIVEQ, MODEQ, LESSEQ, MOREEQ, EQ, IF, AND, OR,

    // multiple char token
    IDENTIFYER, VAR, FOR, WHILE, FUN, RETURN,

    // Literals
    NUM, STRING, NIL

} token_type_e;

typedef struct token {
    token_type_e type;
    const char *text;
    size_t index;
} token_s;

typedef struct token_list {
    size_t size;
    size_t items;
    token_s *array;
} token_list_s;

MINIREPL_DEF int create_token_list(token_list_s **list);
MINIREPL_DEF void delete_token_list(token_list_s **list);
MINIREPL_DEF void print_token_list(token_list_s *list);
MINIREPL_DEF int tokenize_program_string(const char *string, token_list_s *list);

#endif
