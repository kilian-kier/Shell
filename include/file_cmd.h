#ifndef SHELL_FILE_CMD_H
#define SHELL_FILE_CMD_H

#include "main.h"
#include "struct.h"

void cat(struct cmd_line *line);

void touch(struct cmd_line *line);

void rm(struct cmd_line *line);

void mv(struct cmd_line *line);

void cp(struct cmd_line *line);

void chmode(struct cmd_line *line);

void write_to_file(struct cmd_line *line);

void rn(struct cmd_line *line);

#endif //SHELL_FILE_CMD_H
