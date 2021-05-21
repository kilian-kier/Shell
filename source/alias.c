#include "../include/alias.h"


alias *set_alias(alias *node, char *string, command *cmd) {
    if (node == NULL) {
        node = malloc(sizeof(alias));
        node->alias = malloc(strlen(string) * sizeof(char));
        strcpy(node->alias, string);
        node->left = NULL;
        node->right = NULL;
        int pos = ftell(global_alias_file);
        fprintf(global_alias_file, "%s %s\n", cmd->cmd, string);
        if (global_alias_path == NULL)
            fseek(global_alias_file, pos, SEEK_SET);
        return node;
    }
    int x = strcmp(node->alias, string);
    if (x == 0)
        fputs("Dieser Alias ist bereit vorhanden\n", stderr);
    else if (x < 0)
        node->left = set_alias(node->left, string, cmd);
    else
        node->right = set_alias(node->right, string, cmd);
    return node;
}

alias *get_min_right(alias *node) {
    if (node->left == NULL)
        return node;
    return get_min_right(node->left);
}

alias *delete_alias(alias *node, char *string, command *cmd) {
    if (node == NULL) {
        fputs("Dieser Alias ist nicht vorhanden\n", stderr);
        return node;
    }
    int x = strcmp(node->alias, string);
    if (x < 0)
        node->left = delete_alias(node->left, string, cmd);
    else if (x > 0)
        node->right = delete_alias(node->right, string, cmd);
    else {
        if (node->left == NULL) {
            alias *temp = node;
            node = node->right;
            free(temp);
        } else if (node->right == NULL) {
            alias *temp = node;
            node = node->right;
            free(temp);
        } else {
            alias *temp = get_min_right(node->right);
            node->alias = temp->alias;
            node->right = delete_alias(node->right, temp->alias, cmd);
        }
        fseek(global_alias_file, 0, SEEK_END);
        int size = ftell(global_alias_file) - 2;
        rewind(global_alias_file);
        char *buffer = malloc(size * sizeof(char));
        fread(buffer, sizeof(char), size, global_alias_file);
        char *subStr = malloc((strlen(string) + strlen(cmd->cmd)) * sizeof(char));
        strcpy(subStr, cmd->cmd);
        subStr = strcat(strcat(subStr, " "), string);
        char *del = strstr(buffer, subStr);
        int len = (int) strlen(subStr) + 1;
        char *buffer2 = calloc((strlen(buffer) - len), sizeof(char));
        strncpy(buffer2, buffer, del - buffer);
        strcat(buffer2, del + len);
        fclose(global_alias_file);
        global_alias_file = fopen(global_alias_path, "w+");
        fwrite(buffer2, sizeof(char), strlen(buffer2), global_alias_file);
    }
    return node;
}

alias *search_alias(alias *node, char *string) {
    if (node == NULL)
        return NULL;
    int x = strcmp(string, node->alias);
    if (x == 0)
        return node;
    else if (x > 0)
        return search_alias(node->left, string);
    else
        return search_alias(node->right, string);
}