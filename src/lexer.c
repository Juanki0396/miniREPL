#include "lexer.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>



// -----------  token_s methods ------------------------

const char* token_type_to_str(token_type_e token);

#define CASE_ENUM(enum) case enum: return #enum

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
        CASE_ENUM(NOTEQ);
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
        case COMMENT:
        default: 
            return "";
    }
}

// -----------  token_list_s methods ------------------------

token_list_s push_token(token_s tok, token_list_s list);
token_s pop_token(token_list_s list);

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

// ---------------------- TOKENIZATION ----------------------------------

typedef struct str_iter {
    const char* str;
    size_t line_n;
    size_t line_start;
    size_t tok_start;
    size_t tok_len;
} str_iter_s;

str_iter_s create_iter(const char* str);
char next(str_iter_s* iter);
bool match(str_iter_s* iter, char c);
void reset_token(str_iter_s* iter);
void ignore_comment(str_iter_s* iter);
token_list_s push_string(str_iter_s* iter, token_list_s list);
token_list_s push_keyword_or_identifier(str_iter_s* iter, token_list_s list, const char* kewword);
void print_error(str_iter_s* iter, size_t line_n, size_t char_n, const char* message);

str_iter_s create_iter(const char* str){
    return (str_iter_s){ 
        .str = str,
        .line_n = 0,
        .line_start = 0,
        .tok_start = 0,
        .tok_len = 0,
    };
}

inline char get_n_next_char(const str_iter_s iter, size_t n) {
    return iter.str[iter.tok_start + iter.tok_len + n - 1];
}

inline char get_current_char(const str_iter_s iter) {
    return get_n_next_char(iter, 0);
}

inline char get_next_char(const str_iter_s iter) {
    return get_n_next_char(iter, 1);
}

char next(str_iter_s *iter) {
    if ( get_current_char(*iter) == 0) {
        return 0;
    }
    iter->tok_len += 1;
    return get_current_char(*iter);
}

bool match(str_iter_s* iter, char c) {
    if (get_next_char(*iter) == c) {
        next(iter);
        return true;
    } else{
        return false;
    }
}

void reset_token(str_iter_s* iter) {
    iter->tok_start += iter->tok_len;
    iter->tok_len = 1;
}

token_list_s tokenize_program_string(const char* string, token_list_s list) {
    if (!list.valid || list.array == NULL) {
        return INVALID_TOKEN_LIST;
    }
    for ( str_iter_s iter = create_iter(string); get_current_char(iter) != 0; next(&iter)) {
        char c = get_current_char(iter);
        switch (c) {
            case '+':
                list = push_token((token_s){match(&iter, '=') ? ADDEQ : ADD, "", iter.tok_start, 1}, list);
                break;
            case '-':
                list = push_token((token_s){match(&iter, '=') ? SUBEQ : SUB, "", iter.tok_start, 1}, list);
                break;
            case '*':
                list = push_token((token_s){match(&iter, '=') ? MULEQ : MUL, "", iter.tok_start, 1}, list);
                break;
            case '/':
                list = push_token((token_s){match(&iter, '=') ? DIVEQ : DIV, "", iter.tok_start, 1}, list);
                break;
            case '%':
                list = push_token((token_s){match(&iter, '=') ? MODEQ : MOD, "", iter.tok_start, 1}, list);
                break;
            case '=':
                list = push_token((token_s){match(&iter, '=') ? EQ : ASSIGN, "", iter.tok_start, 1}, list);
                break;
            case '!':
                list = push_token((token_s){match(&iter, '=') ? NOTEQ : NEG, "", iter.tok_start, 1}, list);
                break;
            case '>':
                list = push_token((token_s){match(&iter, '=') ? MOREEQ : MORE, "", iter.tok_start, 1}, list);
                break;
            case '<':
                list = push_token((token_s){match(&iter, '=') ? LESSEQ : LESS, "", iter.tok_start, 1}, list);
                break;
            case '#':
                ignore_comment(&iter);
                break;
            case '(':
                list = push_token((token_s){ LEFT_PAR, "", iter.tok_start, 1}, list);
                break;
            case ')':
                list = push_token((token_s){ RIGHT_PAR, "", iter.tok_start, 1}, list);
                break;
            case '{':
                list = push_token((token_s){ LEFT_BRA, "", iter.tok_start, 1}, list);
                break;
            case '}':
                list = push_token((token_s){ RIGHT_BRA, "", iter.tok_start, 1}, list);
                break;
            case ';':
                list = push_token((token_s){ END_STATEMENT, "", iter.tok_start, 1}, list);
                 break;
            case '"':
                list = push_string(&iter, list);
                 break;
            case 'a':
                list = push_keyword_or_identifier(&iter, list, "and");
                 break;
            case 'd':
                list = push_keyword_or_identifier(&iter, list, "def");
                 break;
            case 'f':
                list = push_keyword_or_identifier(&iter, list, "for");
                 break;
            case 'i':
                list = push_keyword_or_identifier(&iter, list, "if");
                 break;
            default: 
                 //check for digit
                 //identifier
                 //error
                 break;
        }
        reset_token(&iter);
    }
    return list;
}

