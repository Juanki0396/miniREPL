#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <strings.h>

#define MINIREPL_IMPL
#include "miniREPL.h"

#define PROGRAM_ARGS 2
#define VERSION "0.1.0"
#define MAX_INPUT_CHARS 100
#define INPUT_FORMAT " %100[^\n]"


int parse_args(int argc, char **argv, bool *verbose, bool *help) {
    if (argc > (PROGRAM_ARGS + 1)) {
        errno = E2BIG;
        return -1;
    }
    for (int idx = 1; idx < argc && argv[idx]; idx++) {
        if (!strcmp(argv[idx], "-h") || !strcmp(argv[idx], "--help")) {
            *help = true;
        }
        else if (!strcmp(argv[idx], "-v") || !strcmp(argv[idx], "--verbose")) {
            *verbose = true;
        }
        else {
            errno = EINVAL;
            return -1;
        }
    }
    return 0;
}

int main(int argc, char* *argv) {
    // Parsing arguments
    printf("miniREPL "VERSION"\n");
    
    bool verbose = false;
    bool help = false;

    if (parse_args(argc, argv, &verbose, &help) == -1) {
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
    if (help) {
        printf("Help\n");
        return EXIT_SUCCESS;
    }
    if (verbose) {
        printf("VERBOSE ON\n");
    }

    // Main loop
    char buffer[MAX_INPUT_CHARS + 1] = { [0 ... MAX_INPUT_CHARS - 1] = 0 };
    while (true) {
        // Read User input
        printf(">>>> ");
        errno = 0;
        if (scanf(INPUT_FORMAT, buffer) == EOF) {
            perror("ERROR invalid input");
            return EXIT_FAILURE;
        }

        // Eval user input
        if (!strcasecmp(buffer, "quit")) {
            break;
        }

        // Print 
        printf("INPUT: %s\n", buffer);
    }

    return EXIT_SUCCESS;
}
