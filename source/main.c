#include "../include/main.h"

int main() {
    void
    (*cmd_functions[])(struct cmd_line *) = {stop, help, clear, cat, working_dir, ls, touch, rm, cd, mv, cp, makedir,
                                             echo, chmode, username, insert_alias, rm_alias, show_aliases,
                                             date, time_now, write_to_file};
    int n = sizeof(cmd_functions) / sizeof(void *);

    extern int *_binary_help_txt_start;
    char *help = (char *) &_binary_help_txt_start;

    char *cmd_token = strtok(help, " ");
    char *help_token;
    for (int i = 0; i < n; i++) {
        help_token = strtok(NULL, "\n");
        global_cmd_root = create_cmd(cmd_token, cmd_functions[i], help_token, global_cmd_root);
        cmd_token = strtok(NULL, " ");
    }
    global_alias_file = fopen("aliases", "r+");
    if (global_alias_file == NULL) {
        global_alias_file = fopen("aliases", "w+");
    }
    global_alias_path = NULL;
    int pos;

    while (fgetc(global_alias_file) != EOF) {
        fseek(global_alias_file, -1, SEEK_CUR);
        pos = count_line(global_alias_file);
        char *line = malloc(pos * sizeof(char));
        fgets(line, pos, global_alias_file);
        char *cmd = strtok(line, " ");
        char *alias = strtok(NULL, " \n\0");
        command *temp = search_cmd(cmd, global_cmd_root);
        temp->aliases = set_alias(temp->aliases, alias, search_cmd(cmd, global_cmd_root));
        free(line);
    }

    global_alias_path = malloc(PATH_MAX + 1);
    realpath("aliases", global_alias_path);

    char *username = whoami();
    printf("Hallo %s\n\n", username);
    FILE *history = fopen("history.txt", "w");
    while (1) {
        printf("%s@%s\n>", username, pwd());
        input(history);
    }
}

int count_line(FILE *f) {
    int start = ftell(f);
    int c = 0, ret = 0;
    while (c != '\n' & c != EOF) {
        c = fgetc(f);
        ret++;
    }
    fseek(f, start, SEEK_SET);
    return ret + 1;
}

#ifdef WIN

char *realpath(char *relative, char *absolut) {
    char *buffer = malloc(PATH_MAX * sizeof(char));
    GetFullPathNameA(relative, PATH_MAX, absolut, &buffer);
    return buffer;
}

#endif