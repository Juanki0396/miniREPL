#include <assert.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>

#include "interpreter.h"
#include "lexer.h"

#define MAX_INPUT_CHARS 100
#define INPUT_FORMAT " %100[^\n]"

int parse_args(int argc, char **argv, args_s *args) {
    if (argc > (PROGRAM_ARGS + 1)) {
        errno = E2BIG;
        return -1;
    }
    for (int idx = 1; idx < (argc - 1) && argv[idx]; idx++) {
        if (!strcmp(argv[idx], "-h") || !strcmp(argv[idx], "--help")) {
            args->help = true;
        }
        else if (!strcmp(argv[idx], "-v") || !strcmp(argv[idx], "--verbose")) {
            args->verbose = true;
        }
        else {
            errno = EINVAL;
            return -1;
        }
    }

    const char *last_arg = argv[argc - 1];
    if (!strcmp(last_arg, "-h") || !strcmp(last_arg, "--help")) {
        args->help = true;
    }
    else if (!strcmp(last_arg, "-v") || !strcmp(last_arg, "--verbose")) {
        args->verbose = true;
    } 
    else if (isalpha(last_arg[0]) || isalnum(last_arg[0])) {
        args->path = last_arg;
    }
    else {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

int run_file(args_s *args) {
    printf("PATH: %s\n", args->path);
    assert(0);
    return 0;
}

int run_interactive(args_s *args) {
    char buffer[MAX_INPUT_CHARS + 1] = { [0 ... MAX_INPUT_CHARS - 1] = 0 };
    while (true) {
        // Read User input
        printf(">>>> ");
        errno = 0;
        if (scanf(INPUT_FORMAT, buffer) == EOF) {
            perror("ERROR invalid input");
            return 1;
        }

        // Eval user input
        if (!strcasecmp(buffer, "quit")) {
            break;
        }

        token_list_s *list;
        if (create_token_list(&list) != 0) {
            perror("ERROR Cannot allocate memory for the parser.");
            return 1;
        }
        if (tokenize_program_string(buffer, list) != 0) {
            perror("ERROR Invalid code.");
            return 1;
        }

        // Print 
        if ( args->verbose ) {
            printf("INPUT TOKENS: \n");
            print_token_list(list);
        }
    }
    return 0;
}
