#include <errno.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"
#include "lexer.h"

#define MAX_INPUT_CHARS 100
#define INPUT_FORMAT " %100[^\n]"
#define DEFAULT_ARGS (args_s){ .verbose=false, .help=false, .has_path=false, .path="" }


args_s parse_args(int argc, char **argv) {
    args_s args = { 0 };
    if (argc > (PROGRAM_ARGS + 1)) {
        fprintf(stderr,"ERROR: Too many arguments.\n");
        exit(1);
    }

    if (argc == 1) {
        return DEFAULT_ARGS;
    }

    for (int idx = 1; idx < (argc - 1) && argv[idx]; idx++) {
        if (!strcmp(argv[idx], "-h") || !strcmp(argv[idx], "--help")) {
            args.help = true;
        }
        else if (!strcmp(argv[idx], "-v") || !strcmp(argv[idx], "--verbose")) {
            args.verbose = true;
        }
        else {
            if (argv[idx][0] == '-') {
                fprintf(stderr,"ERROR: Invalid flag %s.\n", argv[idx]);
            } else {
                fprintf(stderr,"ERROR: Path should be last argument.\n");
            }
            exit(1);
        }
    }

    const char *last_arg = argv[argc - 1];
    if (!strcmp(last_arg, "-h") || !strcmp(last_arg, "--help")) {
        args.help = true;
    }
    else if (!strcmp(last_arg, "-v") || !strcmp(last_arg, "--verbose")) {
        args.verbose = true;
    } 
    else if (isalpha(last_arg[0]) || isalnum(last_arg[0])) {
        args.has_path = true;
        args.path = last_arg;
    }
    else {
        fprintf(stderr,"ERROR: Invalid flag %s.\n", last_arg);
        exit(1);
    }

    return args;
}

unsigned long get_file_size(FILE* fp, const char* path);

unsigned long get_file_size(FILE* fp, const char* path) {
    rewind(fp);
    if( fseek(fp, 0L, SEEK_END) == -1 ) {
        fprintf(stderr, "Cannot compute the size of file %s.\n", path);
        fclose(fp);
        exit(1);
    }
    long size = ftell(fp);
    if (size < 0) {
        fprintf(stderr, "Cannot compute the size of file %s.\n", path);
        fclose(fp);
        exit(1);
    }
    rewind(fp);
    return size;
}

int run_file(args_s args) {
    if ( args.verbose ) {
        printf("PATH: %s\n", args.path);
    }

    FILE* code_file = fopen(args.path, "r");

    if (code_file == NULL) {
        fprintf(stderr, "Cannot open file %s.\n", args.path);
        exit(1);
    }
    unsigned long size = get_file_size(code_file, args.path);
    if (args.verbose) {
        printf("The file %s has a size of %ld bytes.\n", args.path, size * sizeof(char));
    }
    char* code = malloc((size + 1) * sizeof(char));
    memset(code, 0, (size + 1) * sizeof(char));
    if ( size * sizeof(char) != fread(code, sizeof(char), size, code_file)) {
        fprintf(stderr, "Error while reading the file %s.\n", args.path);
        fclose(code_file);
        free(code);
        exit(1);
    }

    if ( args.verbose ) {
        printf("%s", code);
    }

    token_list_s list = create_token_list();
    list = tokenize_program_string(code, list) ;
    if ( args.verbose ) {
        printf("INPUT TOKENS: \n");
        print_token_list(list);
    }

    return 0;
}

int run_interactive(args_s args) {
    char buffer[MAX_INPUT_CHARS + 1] = { [0 ... MAX_INPUT_CHARS - 1] = 0 };
    token_list_s list = create_token_list();
    while (true) {
        // Read User input
        printf(">>>> ");
        errno = 0;
        if (scanf(INPUT_FORMAT, buffer) == EOF) {
            perror("ERROR invalid input");
            exit(1);
        }

        printf("%s\n", buffer);

        // Eval user input
        if (!strcasecmp(buffer, "quit")) {
            break;
        }

        list = tokenize_program_string(buffer, list) ;

        // Print 
        if ( args.verbose ) {
            printf("INPUT TOKENS: \n");
            print_token_list(list);
        }
    }
    return 0;
}
