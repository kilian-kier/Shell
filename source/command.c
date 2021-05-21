#include "../include/command.h"

command *create_cmd(char *cmd, void (*fun)(cmd_line *), char *help, command *node) {
    if (node == NULL) {
        node = malloc(sizeof(command));
        node->cmd = cmd;
        node->cmd_functions = fun;
        node->help = help;
        node->aliases = NULL;
        node->left = NULL;
        node->right = NULL;
        return node;
    }
    if (strcmp(cmd, node->cmd) < 0)
        node->left = create_cmd(cmd, fun, help, node->left);
    else
        node->right = create_cmd(cmd, fun, help, node->right);
    return node;
}

command *search_cmd(char *cmd, command *node) {
    if (node == NULL)
        return NULL;
    if (search_alias(node->aliases, cmd) != NULL)
        return node;
    int x = strcmp(cmd, node->cmd);
    if (x == 0)
        return node;
    else if (x < 0)
        return search_cmd(cmd, node->left);
    else
        return search_cmd(cmd, node->right);
}

void print_help(command *node) {
    if (node == NULL)
        return;
    const char *padding = "...............................";
    char *parameter, *help;
    if (node->help[0] == ':') {
        parameter = "";
        help = strtok(node->help, ":");
    } else {
        parameter = strtok(node->help, ":");
        help = strtok(NULL, ":");
    }
    if (help == NULL)
        help = "\0";
    print_help(node->left);
    printf("%s %s%*.*s :%s\n", node->cmd, parameter, 15 - (int) strlen(node->cmd) - (int) strlen(parameter),
           15 - (int) strlen(node->cmd) - (int) strlen(parameter), padding, help);
    print_help(node->right);
}
