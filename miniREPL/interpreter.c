#include <errno.h>
#include <string.h>

#include "interpreter.h"

#define PROGRAM_ARGS 2

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
