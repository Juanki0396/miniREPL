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
        .valid = true,
    };
    if (list.array == NULL) {
        fprintf(stderr, "Cannot allocate memory for the token list.\n");
        exit(1);
    }
    return list;
}

token_list_s push_token(token_s tok, token_list_s list) {
    if (list.items + 1 > list.size) {
        printf("List to small. Making it bigger\n");
        size_t new_size = list.size + DEFAULT_N_TOKENS;
        list.array = (token_s *)realloc(list.array, sizeof(token_s) * new_size);
        if (list.array == NULL) {
            fprintf(stderr, "Cannot allocate memory for the token list.\n");
            exit(1);
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
    bool valid;
} str_iter_s;

str_iter_s create_iter(const char* str);
char next(str_iter_s* iter);
bool match(str_iter_s* iter, char c);
void reset_token(str_iter_s* iter);
void ignore_comment(str_iter_s* iter);
token_list_s push_string(str_iter_s* iter, token_list_s list);
token_list_s push_num(str_iter_s* iter, token_list_s list);
token_list_s push_keyword_or_identifier(str_iter_s* iter, token_list_s list);
void print_error(str_iter_s iter, const char* message);

str_iter_s create_iter(const char* str){
    return (str_iter_s){ 
        .str = str,
            .line_n = 0,
            .line_start = 0,
            .tok_start = 0,
            .tok_len = 0,
            .valid = true,
    };
}

static inline char get_n_next_char(const str_iter_s iter, size_t n) {
    return iter.str[iter.tok_start + iter.tok_len + n - 1];
}

static inline char get_current_char(const str_iter_s iter) {
    return get_n_next_char(iter, 0);
}

static inline char get_next_char(const str_iter_s iter) {
    return get_n_next_char(iter, 1);
}

static inline bool isidentchar(char c) {
    return (isalnum(c) || c == '_') ? true : false;
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

void ignore_comment(str_iter_s* iter) {
    while (next(iter) != '\n') {
        if (get_current_char(*iter) == 0) {
            return;
        }
    }
    iter->line_n += 1;
    iter->line_start = iter->tok_start + iter->tok_len;
}

#define STR_BUFFER_SIZE 64

token_list_s push_string(str_iter_s* iter, token_list_s list) {
    char*  buffer = malloc(sizeof(char) * STR_BUFFER_SIZE);
    size_t size = STR_BUFFER_SIZE;
    size_t idx = 0;
    if (buffer == NULL) {
        fprintf(stderr, "Cannot allocate memory to hold string.\n") ;
        exit(1);
    }
    memset(buffer, 0, STR_BUFFER_SIZE);
    while (next(iter) != '"') {
        if (get_current_char(*iter) == 0) {
            print_error(*iter, "Unclosed string");
            exit(1);
        } else if (get_current_char(*iter) == '\n') {
            iter->line_n += 1;
            iter->line_start = iter->tok_start + iter->tok_len;
        } else {
            if (idx == size - 1) {
                size += STR_BUFFER_SIZE;
                buffer = realloc((void*)buffer, sizeof(char) * size);
                if (buffer == NULL) {
                    fprintf(stderr, "Cannot allocate memory to hold string.\n") ;
                    exit(1);
                }
                memset(&buffer[idx + 1], 0, STR_BUFFER_SIZE);
            }
            buffer[idx] = get_current_char(*iter);
            idx += 1;
        }
    }
    return push_token((token_s){ STRING, { .str=buffer }, iter->tok_start, 1}, list);
}

token_list_s push_num(str_iter_s* iter, token_list_s list) {
    char buffer[STR_BUFFER_SIZE * 2] = { 0 };
    size_t idx = 0;
    bool decimals = false;
    char c = get_current_char(*iter);
    for (;;) {
        if (isdigit(c)) {
            buffer[idx] = c;
        } else if ( c == '.' && !decimals ) {
            buffer[idx] = c;
        } else if ( c == '.' && decimals ) {
            print_error(*iter, "Number already have a decimal delimeter.\n") ;
            exit(1);
        }
        c = get_next_char(*iter);
        if (isdigit(c) || c == '.') {
            next(iter);
            idx += 1;
            if(idx == STR_BUFFER_SIZE*2 - 1) {
                char err_msg[256];
                sprintf(err_msg, "Number precission too big. Max digits allowed are %u.\n", STR_BUFFER_SIZE * 2 );
                print_error(*iter, err_msg) ;
                exit(1);
            }
        } else {
            break;
        }
    }
    return push_token((token_s){ NUM, { .num=atof(buffer) }, iter->tok_start, 1}, list);
}

token_list_s push_keyword_or_identifier(str_iter_s* iter, token_list_s list) {
    char c = get_current_char(*iter);
    for (;;) {
        if( isidentchar(c) && isidentchar(get_next_char(*iter)) ) {
            next(iter);
        } else {
            break;
        }
    }
    if (iter->tok_len > STR_BUFFER_SIZE - 1) {
        char err_msg[256];
        sprintf(err_msg, "Identifier exceed maximum length. Max chars allowed are %u.\n", STR_BUFFER_SIZE );
        print_error(*iter, err_msg) ;
        exit(1);
    }
    char word[STR_BUFFER_SIZE] = { 0 };
    memcpy(word, &iter->str[iter->tok_start], iter->tok_len);
    if (strcmp(word, "and")  == 0) {
        return push_token((token_s){ AND, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "def")) {
        return push_token((token_s){ FUN, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "for")) {
        return push_token((token_s){ FOR, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "if")) {
        return push_token((token_s){ IF, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "nil")) {
        return push_token((token_s){ NIL, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "or")) {
        return push_token((token_s){ OR, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "return")) {
        return push_token((token_s){ RETURN, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "var")) {
        return push_token((token_s){ VAR, { 0 }, iter->tok_start, 1}, list);
    } else if (strcmp(word, "while")) {
        return push_token((token_s){ WHILE, { 0 }, iter->tok_start, 1}, list);
    } else {
        return push_token((token_s){ IDENTIFYER, { .str=word }, iter->tok_start, 1}, list);
    }
}

void print_error(str_iter_s iter, const char* message) {
    fprintf(stderr, "Error in line %zu:%zu: %s\n",
            iter.line_n,
            (iter.tok_start + iter.tok_len) - iter.line_start,
            message);
}

token_list_s tokenize_program_string(const char* string, token_list_s list) {
    if (!list.valid || list.array == NULL) {
        fprintf(stderr, "Invalid token string.\n") ;
        exit(1);
    }
    for ( str_iter_s iter = create_iter(string);
            get_current_char(iter) != 0 && iter.valid; 
            next(&iter)) {
        char c = get_current_char(iter);
        switch (c) {
            case '+':
                list = push_token((token_s){match(&iter, '=') ? ADDEQ : ADD, { 0 }, iter.tok_start, 1}, list);
                break;
            case '-':
                list = push_token((token_s){match(&iter, '=') ? SUBEQ : SUB, { 0 }, iter.tok_start, 1}, list);
                break;
            case '*':
                list = push_token((token_s){match(&iter, '=') ? MULEQ : MUL, { 0 }, iter.tok_start, 1}, list);
                break;
            case '/':
                list = push_token((token_s){match(&iter, '=') ? DIVEQ : DIV, { 0 }, iter.tok_start, 1}, list);
                break;
            case '%':
                list = push_token((token_s){match(&iter, '=') ? MODEQ : MOD, { 0 }, iter.tok_start, 1}, list);
                break;
            case '=':
                list = push_token((token_s){match(&iter, '=') ? EQ : ASSIGN, { 0 }, iter.tok_start, 1}, list);
                break;
            case '!':
                list = push_token((token_s){match(&iter, '=') ? NOTEQ : NEG, { 0 }, iter.tok_start, 1}, list);
                break;
            case '>':
                list = push_token((token_s){match(&iter, '=') ? MOREEQ : MORE, { 0 }, iter.tok_start, 1}, list);
                break;
            case '<':
                list = push_token((token_s){match(&iter, '=') ? LESSEQ : LESS, { 0 }, iter.tok_start, 1}, list);
                break;
            case '#':
                ignore_comment(&iter);
                break;
            case '(':
                list = push_token((token_s){ LEFT_PAR, { 0 }, iter.tok_start, 1}, list);
                break;
            case ')':
                list = push_token((token_s){ RIGHT_PAR, { 0 }, iter.tok_start, 1}, list);
                break;
            case '{':
                list = push_token((token_s){ LEFT_BRA, { 0 }, iter.tok_start, 1}, list);
                break;
            case '}':
                list = push_token((token_s){ RIGHT_BRA, { 0 }, iter.tok_start, 1}, list);
                break;
            case ';':
                list = push_token((token_s){ END_STATEMENT, { 0 }, iter.tok_start, 1}, list);
                break;
            case '"':
                list = push_string(&iter, list);
                break;
            default: 
                if (isdigit(c)) {
                    list = push_num(&iter, list);
                } else if (isalpha(c)) {
                    list = push_keyword_or_identifier(&iter, list);
                } else {
                    print_error(iter, "Invalid token");
                    exit(1);
                };
                break;
        }
        reset_token(&iter);
    }
    return list;
}

