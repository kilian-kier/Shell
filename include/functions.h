#ifndef SHELL_FUNCTIONS_H
#define SHELL_FUNCTIONS_H

#include <limits.h>
#include "main.h"

char *pwd();

int file_size(char *filename);

int file_blksize(char *filename);

char *file_owner(char *filename);

char *file_group(char *filename);

char *last_change(char *filename);

char *file_permission(char *filename);

char *whoami();

int copy_file(const char *old_file, const char *new_file);

char *destination_path(const char *dir, const char *name);

int convertChmod(int x);

#endif //SHELL_FUNCTIONS_H
