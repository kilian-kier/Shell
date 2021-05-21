#ifndef SHELL_BASIC_CMD_H
#define SHELL_BASIC_CMD_H

#include "main.h"
#include "struct.h"
#include "functions.h"

void stop(struct cmd_line *line);

void help(struct cmd_line *line);

void clear(struct cmd_line *line);

void working_dir(struct cmd_line *line);

void ls(struct cmd_line *line);

void cd(struct cmd_line *line);

void makedir(struct cmd_line *line);

void echo(struct cmd_line *line);

void username(struct cmd_line *line);

void date(struct cmd_line *line);

void time_now(struct cmd_line *line);

void show_aliases(struct cmd_line *line);

void insert_alias(struct cmd_line *line);

void rm_alias(struct cmd_line *line);

#endif //SHELL_BASIC_CMD_H
