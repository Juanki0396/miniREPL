#ifndef MINIREPL_H
#define MINIREPL_H

#define MINIREPL_DEF 
#define DEFAULT_N_TOKENS 100

#include <stddef.h>
#include <stdbool.h>

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
    const char* text;
    size_t index;
    bool valid;
} token_s;

typedef struct token_list {
    size_t size;
    size_t items;
    token_s* array;
    bool valid;
} token_list_s;

#define LEXER_ERR int
#define LEXER_SUCCES 0
#define ALLOC_ERR 1
#define SYNTAX_ERR 2

#define INVALID_TOKEN (token_s) { 0 }
#define INVALID_TOKEN_LIST (token_list_s) { 0 }

token_list_s create_token_list(void);
void delete_token_list(token_list_s list);
void print_token_list(token_list_s list);
token_list_s tokenize_program_string(const char* string);

#endif
