#include "../include/struct.h"

cmd_line *create_line(int n, int size, char *args[]) {
    cmd_line *ret = malloc(sizeof(cmd_line));
    ret->argc = n;
    ret->argv = malloc(size * sizeof(char));
    ret->argv = args;
    return ret;
}