#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdbool.h>

#define PROGRAM_ARGS 3

typedef struct args {
    bool verbose;
    bool help;
    const char *path;
} args_s;

int parse_args(int argc, char **argv, args_s *args);
int run_file(args_s *args);
int run_interactive(args_s *args);

#endif
