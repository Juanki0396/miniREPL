#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define CASE_ENUM(enum) case enum: return #enum

typedef struct str_iter {
    const char* str;
    size_t tok_start;
    size_t tok_len;
} str_iter_s;

const char* token_type_to_str(token_type_e token);
token_list_s push_token(token_s tok, token_list_s list);
token_s pop_token(token_list_s list);
token_s get_token(size_t idx, token_list_s list);
void next_token(str_iter_s *iter);

const char * token_type_to_str(token_type_e token){
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

token_list_s create_token_list() {
    token_list_s list = { 
        .size = DEFAULT_N_TOKENS, 
        .items = 0,
        .array =(token_s *)calloc(DEFAULT_N_TOKENS, sizeof(token_s)),
    };
    if (list.array == NULL) {
        return INVALID_TOKEN_LIST;
    }
    return list;
}

token_list_s push_token(token_s tok, token_list_s list) {
    if (list.items + 1 > list.size) {
        printf("List to small. Making it bigger\n");
        size_t new_size = list.size + DEFAULT_N_TOKENS;
        list.array = (token_s *)realloc(list.array, sizeof(token_s) * new_size);
        if (list.array == NULL) {
            return INVALID_TOKEN_LIST;
        }
        list.size = new_size;
        memset(&list.array[list.items], 0, sizeof(token_s) * DEFAULT_N_TOKENS);
    }
    list.items += 1;
    printf("Inserting token\n");
    list.array[list.items - 1] = tok;
    return list;
}

token_s pop_token(token_list_s list) {
    if (list.items <= 0) {
        return INVALID_TOKEN;
    }
    token_s tok = list.array[list.items - 1];
    memset(&list.array[list.items - 1], 0, sizeof(token_s));
    list.items -= 1;
    return tok;
}

token_s get_token(size_t idx, token_list_s list) {
    if (idx >= list.items) {
        return INVALID_TOKEN;
    }
    return list.array[idx];
}

void delete_token_list(token_list_s list) {
    if (list.array == NULL) {
        free(list.array);
    }
}

void print_token_list(token_list_s list) {
    printf("(\n\t");
    for(size_t i = 1; i <= list.items; i++) {
       printf(" %s ", token_type_to_str(list.array[i].type));
       if (i % 5 == 0) {
           printf("\n\t");
       }
    }
    printf("\n)\n");
}

inline char watch_n_next(const str_iter_s *iter, size_t n) {
    return iter->str[iter->tok_start + iter->tok_len + n - 1];
}

inline char watch_next(const str_iter_s *iter) {
    return watch_n_next(iter, 1);
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

token_list_s tokenize_program_string(const char* string) {
    token_list_s list = create_token_list();
    if (!list.valid) {
        return INVALID_TOKEN_LIST;
    }
    size_t len = strlen(string);
    str_iter_s iter = { .str = string, .tok_start = 0, .tok_len = 1, };
    while ( iter.tok_start < len ){
        // 1 char tokens
        if ( watch_current(&iter) == '+' ) {
            token_s tok = { ADD, "", iter.tok_start, 1 };
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '-' ) {
            token_s tok = { SUB, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '*' ) {
            token_s tok = { MUL, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '/' ) {
            token_s tok = { DIV, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '%' ) {
            token_s tok = { MOD, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '=' ) {
            token_s tok = { ASSIGN, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '!' ) {
            token_s tok = { NEG, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '>' ) {
            token_s tok = { MORE, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '<' ) {
            token_s tok = { LESS, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '(' ) {
            token_s tok = { LEFT_PAR, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == ')' ) {
            token_s tok = { RIGHT_PAR, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '{' ) {
            token_s tok = { LEFT_BRA, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == '}' ) {
            token_s tok = { RIGHT_BRA, "", iter.tok_start, 1};
            list = push_token(tok, list);
        } else if ( watch_current(&iter) == ';' ) {
            token_s tok = { END_STATEMENT, "", iter.tok_start, 1};
            list = push_token(tok, list);
        // 2 char tokens
        } else if ( watch_current(&iter) == '+' && watch_next(&iter) == '=' ) {
            token_s tok = { ADDEQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '-' && watch_next(&iter) == '=' ) {
            token_s tok = { SUBEQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '*' && watch_next(&iter) == '=' ) {
            token_s tok = { MULEQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '/' && watch_next(&iter) == '=' ) {
            token_s tok = { DIVEQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '%' && watch_next(&iter) == '=' ) {
            token_s tok = { MODEQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '<' && watch_next(&iter) == '=' ) {
            token_s tok = { LESSEQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '>' && watch_next(&iter) == '=' ) {
            token_s tok = { MOREEQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '=' && watch_next(&iter) == '=' ) {
            token_s tok = { EQ, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '&' && watch_next(&iter) == '&' ) {
            token_s tok = { AND, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == '|' && watch_next(&iter) == '|' ) {
            token_s tok = { OR, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        // reserved keywords tokens
        } else if ( watch_current(&iter) == 'i' &&
                watch_next(&iter) == 'f' &&
                isspace(watch_n_next(&iter, 2)) ) {
            token_s tok = { IF, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'i' &&
                watch_next(&iter) == 'f' &&
                isspace(watch_n_next(&iter, 2)) ) {
            token_s tok = { IF, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'v' &&
                watch_next(&iter) == 'a' &&
                watch_n_next(&iter, 2) == 'r' &&
                isspace(watch_n_next(&iter, 3)) ) {
            token_s tok = { VAR, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'f' &&
                watch_next(&iter) == 'o' &&
                watch_n_next(&iter, 2) == 'r' &&
                isspace(watch_n_next(&iter, 3)) ) {
            token_s tok = { FOR, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'f' &&
                watch_next(&iter) == 'u' &&
                watch_n_next(&iter, 2) == 'n' &&
                isspace(watch_n_next(&iter, 3)) ) {
            token_s tok = { FUN, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'n' &&
                watch_next(&iter) == 'i' &&
                watch_n_next(&iter, 2) == 'l' &&
                isspace(watch_n_next(&iter, 3)) ) {
            token_s tok = { NIL, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'w' &&
                watch_next(&iter) == 'h' &&
                watch_n_next(&iter, 2) == 'i' &&
                watch_n_next(&iter, 3) == 'l' &&
                watch_n_next(&iter, 4) == 'e' &&
                isspace(watch_n_next(&iter, 5)) ) {
            token_s tok = { WHILE, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
            next_char(&iter);
            next_char(&iter);
            next_char(&iter);
        } else if ( watch_current(&iter) == 'r' &&
                watch_next(&iter) == 'e' &&
                watch_n_next(&iter, 2) == 't' &&
                watch_n_next(&iter, 3) == 'u' &&
                watch_n_next(&iter, 4) == 'r' &&
                watch_n_next(&iter, 5) == 'n' &&
                isspace(watch_n_next(&iter, 6)) ) {
            token_s tok = { RETURN, "", iter.tok_start, 1};
            list = push_token(tok, list);
            next_char(&iter);
            next_char(&iter);
            next_char(&iter);
            next_char(&iter);
        // Identifyer, num and string tokens
        } else {
            return INVALID_TOKEN_LIST;
        }
        next_token(&iter);
    }
    return list;
}

