#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <strings.h>

#include "interpreter.h"
#include "lexer.h"

#define VERSION "0.1.0"

int main(int argc, char** argv) {
    // Parsing arguments
    printf("miniREPL "VERSION"\n");
    
    args_s args = parse_args(argc, argv);

    // Handling help and verbose    
    if (args.help) {
        printf("Help\n");
        return EXIT_SUCCESS;
    }
    if (args.verbose) {
        printf("VERBOSE ON\n");
    }

    int result = 0;
    if (args.has_path) {
        result = run_file(args);
    }
    else {
        result = run_interactive(args);
    }

    if (result != 0) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
