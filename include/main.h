#ifndef SHELL_MAIN_H
#define SHELL_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
#include "input.h"
#include "struct.h"
#include "functions.h"

command *global_cmd_root;
FILE *global_alias_file;
char *global_alias_path;

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64)
#define WIN
#define path_separator '\\'
#define clear_cmd "cls"

#include <windows.h>

char *realpath(char *relative, char *absolut);

#else
#define LINUX
#define path_separator '/'
#define clear_cmd "clear"
#include <pwd.h>
#include <grp.h>
#endif

int count_line(FILE *f);

#endif //SHELL_MAIN_H
