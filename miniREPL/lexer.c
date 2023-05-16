#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define CASE_ENUM(enum) case enum: return #enum

typedef struct str_iter {
    const char *str;
    size_t tok_start;
    size_t tok_len;
} str_iter_s;

MINIREPL_DEF const char * token_type_to_str(token_type_e token);
MINIREPL_DEF int push_token(token_s tok, token_list_s *list);
MINIREPL_DEF int pop_token(token_s *tok, token_list_s *list);
MINIREPL_DEF int get_token(size_t idx, token_list_s *list, token_s *tok);
void next_token(str_iter_s *iter);

MINIREPL_DEF const char * token_type_to_str(token_type_e token){
    switch(token){
        CASE_ENUM(ADD);
        CASE_ENUM(SUB);
        CASE_ENUM(MUL);
        CASE_ENUM(DIV);
        CASE_ENUM(MOD);
        CASE_ENUM(ASSIGN);
        CASE_ENUM(NEG);
        CASE_ENUM(MORE);
        CASE_ENUM(LESS);
        CASE_ENUM(LEFT_PAR);
        CASE_ENUM(RIGHT_PAR);
        CASE_ENUM(LEFT_BRA);
        CASE_ENUM(RIGHT_BRA);
        CASE_ENUM(END_STATEMENT);
        CASE_ENUM(ADDEQ);
        CASE_ENUM(SUBEQ);
        CASE_ENUM(MULEQ);
        CASE_ENUM(DIVEQ);
        CASE_ENUM(MODEQ);
        CASE_ENUM(LESSEQ);
        CASE_ENUM(MOREEQ);
        CASE_ENUM(EQ);
        CASE_ENUM(IF);
        CASE_ENUM(AND);
        CASE_ENUM(OR);
        CASE_ENUM(IDENTIFYER);
        CASE_ENUM(VAR);
        CASE_ENUM(FOR);
        CASE_ENUM(FUN);
        CASE_ENUM(WHILE);
        CASE_ENUM(RETURN);
        CASE_ENUM(NUM);
        CASE_ENUM(STRING);
        CASE_ENUM(NIL);
        default: return "";
    }
}

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

MINIREPL_DEF void print_token_list(token_list_s *list) {
    printf("(\n\t");
    for(size_t i = 1; i <= list->items; i++) {
       printf(" %s ", token_type_to_str(list->array[i].type));
       if (i % 5 == 0) {
           printf("\n\t");
       }
    }
    printf("\n)\n");
}

inline char watch_next(const str_iter_s *iter) {
    return iter->str[iter->tok_start + iter->tok_len];
}

inline char watch_current(const str_iter_s *iter) {
    return iter->str[iter->tok_start + iter->tok_len - 1];
}

void next_token(str_iter_s *iter) {
    iter->tok_start += iter->tok_len;
    iter->tok_len = 1;
    size_t len = strlen(iter->str);
    while ( isspace(iter->str[iter->tok_start]) ) {
        if( iter->tok_start >= len ) {
            break; 
        }
        iter->tok_start += 1;
    }
}

inline void next_char(str_iter_s *iter) {
    iter->tok_len += 1;
}

inline const char * current_token(str_iter_s *iter) {
    return &iter->str[iter->tok_start];
}

MINIREPL_DEF int tokenize_program_string(const char *string, token_list_s *list) {
    size_t len = strlen(string);
    str_iter_s iter = { .str = string, .tok_start = 0, .tok_len = 1, };
    while ( iter.tok_start < len ){
        // 1 char tokens
        if ( watch_current(&iter) == '+' ) {
            token_s tok = { ADD, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '-' ) {
            token_s tok = { SUB, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '*' ) {
            token_s tok = { MUL, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '/' ) {
            token_s tok = { DIV, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '%' ) {
            token_s tok = { MOD, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '=' ) {
            token_s tok = { ASSIGN, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '!' ) {
            token_s tok = { NEG, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '>' ) {
            token_s tok = { MORE, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '<' ) {
            token_s tok = { LESS, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '(' ) {
            token_s tok = { LEFT_PAR, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == ')' ) {
            token_s tok = { RIGHT_PAR, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '{' ) {
            token_s tok = { LEFT_BRA, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == '}' ) {
            token_s tok = { RIGHT_BRA, "", iter.tok_start };
            push_token(tok, list);
        } else if ( watch_current(&iter) == ';' ) {
            token_s tok = { END_STATEMENT, "", iter.tok_start };
            push_token(tok, list);
        // 2 char tokens
        } else if ( watch_current(&iter) == '+' && watch_next(&iter) == '=' ) {
            token_s tok = { ADDEQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '-' && watch_next(&iter) == '=' ) {
            token_s tok = { SUBEQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '*' && watch_next(&iter) == '=' ) {
            token_s tok = { MULEQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '/' && watch_next(&iter) == '=' ) {
            token_s tok = { DIVEQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '%' && watch_next(&iter) == '=' ) {
            token_s tok = { MODEQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '<' && watch_next(&iter) == '=' ) {
            token_s tok = { LESSEQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '>' && watch_next(&iter) == '=' ) {
            token_s tok = { MOREEQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '=' && watch_next(&iter) == '=' ) {
            token_s tok = { EQ, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'i' && watch_next(&iter) == 'f' ) {
            token_s tok = { IF, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '&' && watch_next(&iter) == '&' ) {
            token_s tok = { AND, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '|' && watch_next(&iter) == '|' ) {
            token_s tok = { OR, "", iter.tok_start };
            push_token(tok, list);
            next_char(&iter);
        } else {
            return 1;
        }
        next_token(&iter);
    }
    return 0;
}

