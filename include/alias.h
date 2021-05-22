#ifndef SHELL_ALIAS_H
#define SHELL_ALIAS_H

#include "struct.h"
#include "main.h"

alias *set_alias(alias *node, char *string, command *cmd);

alias *delete_alias(alias *node, char *string, command *cmd);

command *search_alias(command *node, char *string);

alias *search_aliases(alias *node, char *string);

#endif //SHELL_ALIAS_H
