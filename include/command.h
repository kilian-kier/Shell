#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#include "struct.h"

command *create_cmd(char *cmd, void (*fun)(cmd_line *), char *help, command *node);

command *search_cmd(char *cmd, command *node);

void print_help(command *node);

#endif //SHELL_COMMAND_H
