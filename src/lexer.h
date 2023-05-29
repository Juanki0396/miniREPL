#ifndef MINIREPL_H
#define MINIREPL_H

#define MINIREPL_DEF 
#define DEFAULT_N_TOKENS 100

#include <stddef.h>
#include <stdbool.h>

typedef enum token_type {
    // 1 char tokens
    ADD, SUB, MUL, DIV, MOD, ASSIGN, NEG, MORE, LESS, LEFT_PAR, RIGHT_PAR,
    LEFT_BRA, RIGHT_BRA, END_STATEMENT, COMMENT,

    // 2 char tokens
    ADDEQ, SUBEQ, MULEQ, DIVEQ, MODEQ, LESSEQ, MOREEQ, EQ, NOTEQ,

    // Reserve words tokens
    IF, VAR, FOR, WHILE, FUN, RETURN, AND, OR, NIL,

    // multiple char token
    IDENTIFYER,

    // Literals
    NUM, STRING

} token_type_e;

typedef union data {
    float num;
    const char* str;
} data_u;

typedef struct token {
    token_type_e type;
    data_u text;
    size_t index;
    bool valid; // true is ok, false cannot be used
} token_s;

typedef struct token_list {
    size_t size;
    size_t items;
    token_s* array;
    bool valid; // true is ok, false cannot be used
} token_list_s;

#define INVALID_TOKEN (token_s) { 0 }
#define INVALID_TOKEN_LIST (token_list_s) { 0 }

// Allocate memory for a new token list and return it. The user is
// responsible to free the memory. It can fail and return and INVALID_TOKEN_LIST
token_list_s create_token_list(void);

// Helper method to free the memory alocated for a list. 
// WARNING: Don't use the object once is passed.
void delete_token_list(token_list_s list);

// Return the token in the specified position. Can fail with an invalid idx
// returning and INVALID_TOKEN
token_s get_token(size_t idx, token_list_s list);

// Print into the stdout the tokens inside the list.
void print_token_list(token_list_s list);

// Parse a string of miniREPL code transforming it into a token_list. It can
// fail and return an INVALID_TOKEN_LIST
token_list_s tokenize_program_string(const char* string, token_list_s list);

#endif
