#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <strings.h>

#include "interpreter.h"
#include "lexer.h"

#define VERSION "0.1.0"

int main(int argc, char* *argv) {
    // Parsing arguments
    printf("miniREPL "VERSION"\n");
    
    args_s args = { .verbose = false, .help = false, .path = "" };

    if (parse_args(argc, argv, &args) == -1) {
        if (errno == EINVAL) {
            perror("ERROR parsing arguments");
            printf("Please use -h or --help for see instructions.\n");
        }
        else if (errno == E2BIG) {
            perror("ERROR parsing arguments");
            printf("Please use -h or --help for see instructions.\n");
        }
        return EXIT_FAILURE;
    }
    errno = 0;

    // Handling help and verbose    
    if (args.help) {
        printf("Help\n");
        return EXIT_SUCCESS;
    }
    if (args.verbose) {
        printf("VERBOSE ON\n");
    }

    int result = 0;
    if (!strcmp(args.path, "") && 0) {
        result = run_file(&args);
    }
    else {
        result = run_interactive(&args);
    }

    if (result != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
