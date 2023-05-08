#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PROGRAM_ARGS 2
#define VERSION "0.1.0"

int parse_args(int argc, char **argv, bool *verbose, bool *help) {
    if (argc > (PROGRAM_ARGS + 1)) {
        return 1;
    }
    for (int idx = 1; idx < argc && argv[idx]; idx++) {
        if (!strcmp(argv[idx], "-h") || !strcmp(argv[idx], "--help")) {
            *help = true;
        }
        else if (!strcmp(argv[idx], "-v") || !strcmp(argv[idx], "--verbose")) {
            *verbose = true;
        }
        else {
            fprintf(stderr, "ERROR: Unknown argument: %s: Please use -h or --help for see instructions.\n", argv[idx]);
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* *argv) {
    printf("miniREPL "VERSION"\n");
    bool verbose = false;
    bool help = false;
    if (parse_args(argc, argv, &verbose, &help)) {
            return 1;
    }
    if (help) {
        printf("Help\n");
        return 0;
    }
    if (verbose) {
        printf("VERBOSE ON\n");
    }
    return 0;
}
