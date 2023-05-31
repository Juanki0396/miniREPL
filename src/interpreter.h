#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#define PROGRAM_ARGS 3

typedef struct args {
    bool verbose;
    bool help;
    bool has_path;
    const char *path;
} args_s;

args_s parse_args(int argc, char **argv);
int run_file(args_s args);
int run_interactive(args_s args);

#endif
