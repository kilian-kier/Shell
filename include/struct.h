#ifndef SHELL_STRUCT_H
#define SHELL_STRUCT_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct cmd_line {
    int argc;
    char **argv;
} cmd_line;

typedef struct alias {
    char *alias;
    struct alias *left;
    struct alias *right;
} alias;

typedef struct command {
    char *cmd;
    alias *aliases;

    void (*cmd_functions)(struct cmd_line *);

    char *help;
    struct command *left;
    struct command *right;
} command;

#include "alias.h"
#include "command.h"

cmd_line *create_line(int n, int size, char *args[]);

#endif //SHELL_STRUCT_H
