#include "../include/basic_cmd.h"

void stop(struct cmd_line *line) {
    exit(0);
}

void help(cmd_line *line) {
    if (line->argc < 2) {
        printf("Diese Befehle kannst du verwenden:\n\n");
        print_help(global_cmd_root);
    } else {
        for (int i = 1; i < line->argc; i++) {
            command *temp = search_cmd(line->argv[i], global_cmd_root);
            const char *padding = "...............................";
            char *parameter, *help;
            if (temp->help[0] == ':') {
                parameter = "";
                help = strtok(temp->help, ":");
            } else {
                parameter = strtok(temp->help, ":");
                help = strtok(NULL, ":");
            }
            if (help == NULL)
                help = "\0";
            printf("%s %s%*.*s :%s\n", temp->cmd, parameter, 15 - (int) strlen(temp->cmd) - (int) strlen(parameter),
                   15 - (int) strlen(temp->cmd) - (int) strlen(parameter), padding, help);
        }
    }
}

void clear(struct cmd_line *line) {
    system(clear_cmd);
}

void working_dir(struct cmd_line *line) {
    printf("%s\n", pwd());
}

#ifdef WIN

void makedir(struct cmd_line *line) {
    if (line->argc < 2) {
        fputs("Bitte uebergib min. einen Parameter\n", stderr);
        return;
    }
    for (int i = 1; i < line->argc; i++) {
        if (mkdir(line->argv[i]))
            perror("mkdir");
    }
}

void ls(struct cmd_line *line) {
    char *current = pwd();
    if (line->argc == 1) {
        cmd_line *temp = malloc(sizeof(cmd_line));
        temp->argv = malloc(5 * sizeof(char));
        temp->argv[0] = "ls";
        temp->argv[1] = ".";
        temp->argc = 2;
        ls(temp);
        return;
    }
    DIR *d;
    struct dirent *files;
    d = opendir(line->argv[1]);
    if (d != NULL) {
        printf("   %-15s", "Size");
        printf(" %-15s", "Size on disk");
        printf(" %-23s", "Last change");
        printf("%s\n\n", "Name");
        if (strcmp(line->argv[1], ".") != 0)
            chdir(line->argv[1]);
        while ((files = readdir(d)) != NULL) {
            if (strcmp(files->d_name, ".") == 0 || strcmp(files->d_name, "..") == 0)
                continue;
            DIR *folder = opendir(files->d_name);
            if (folder != NULL) {
                printf("d:");
                closedir(folder);
            } else {
                printf("f:");
            }
            printf(" %-15d", file_size(files->d_name));
            printf(" %-15d", file_blksize(files->d_name));
            printf(" %-23s", last_change(files->d_name));
            printf("%s\n", files->d_name);
        }
        closedir(d);
        if (strcmp(line->argv[1], ".") != 0)
            chdir(current);
    } else
        fprintf(stderr, "Bitte uebergib einen Ordner als Parameter\n");
}

#else

void makedir(struct cmd_line *line) {
    if (line->argc < 2) {
        fputs("Bitte übergib min. einen Parameter\n", stderr);
        return;
    }
    for (int i = 1; i < line->argc; i++) {
        if (mkdir(line->argv[i], 0777) == -1)
            perror("mkdir");
    }
}

void ls(struct cmd_line *line) {
    char *current = pwd();
    if (line->argc == 1) {
        cmd_line *temp = malloc(sizeof(cmd_line));
        temp->argv = malloc(5 * sizeof(char));
        temp->argv[0] = "ls";
        temp->argv[1] = ".";
        temp->argc = 2;
        ls(temp);
        return;
    }
    DIR *d;
    struct dirent *files;
    d = opendir(line->argv[1]);
    if (d != NULL) {
        printf("   %-15s", "Permissions");
        printf(" %-15s", "Owner");
        printf(" %-15s", "Group");
        printf(" %-15s", "Size");
        printf(" %-15s", "Size on disk");
        printf(" %-23s", "Last change");
        printf("%s\n\n", "Name");
        if (strcmp(line->argv[1], ".") != 0)
            chdir(line->argv[1]);
        while ((files = readdir(d)) != NULL) {
            if (strcmp(files->d_name, ".") == 0 || strcmp(files->d_name, "..") == 0)
                continue;
            DIR *folder = opendir(files->d_name);
            if (folder != NULL) {
                printf("d:");
                closedir(folder);
            } else {
                printf("f:");
            }
            printf(" %-15s", file_permission(files->d_name));
            printf(" %-15s", file_owner(files->d_name));
            printf(" %-15s", file_group(files->d_name));
            printf(" %-15d", file_size(files->d_name));
            printf(" %-15d", file_blksize(files->d_name));
            printf(" %-23s", last_change(files->d_name));
            printf("%s\n", files->d_name);
        }
        closedir(d);
        if (strcmp(line->argv[1], ".") != 0)
            chdir(current);
    } else
        fprintf(stderr, "Bitte übergib einen Ordner als Parameter\n");
}

#endif

void cd(struct cmd_line *line) {
    if (line->argc < 2) {
        fprintf(stderr, "Bitte übergib min. einen Parameter\n");
        return;
    }
    if (chdir(line->argv[1]) != 0)
        perror("cd failed");
}


void echo(struct cmd_line *line) {
    for (int i = 1; i < line->argc; i++) {
        printf("%s ", line->argv[i]);
    }
    printf("\n");
}


void username(struct cmd_line *line) {
    printf("%s\n", whoami());
}

void date(struct cmd_line *line) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d.%02d.%d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void time_now(struct cmd_line *line) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("%02d:%02d:%02d\n", tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void show_aliases(struct cmd_line *line) {
    int pos;
    rewind(global_alias_file);
    while (fgetc(global_alias_file) != EOF) {
        fseek(global_alias_file, -1, SEEK_CUR);
        pos = count_line(global_alias_file);
        char *file_line = malloc(pos * sizeof(char));
        fgets(file_line, pos, global_alias_file);
        char *cmd = strtok(file_line, " ");
        char *alias = strtok(NULL, " \n\0");
        printf("%s - %s\n", cmd, alias);
        free(file_line);
    }
}

void insert_alias(struct cmd_line *line) {
    if (line->argc < 3) {
        fprintf(stderr, "Bitte übergib min. 2 Parameter\n");
        return;
    }
    command *cmd = search_cmd(line->argv[1], global_cmd_root);
    if (cmd == NULL) {
        fputs("Befehl nicht gefunden\n", stderr);
        return;
    }
    for (int i = 2; i < line->argc; i++) {
        cmd->aliases = set_alias(cmd->aliases, line->argv[i], cmd);
    }
}

void rm_alias(struct cmd_line *line) {
    if (line->argc < 3) {
        fprintf(stderr, "Bitte übergib min. 2 Parameter\n");
        return;
    }
    command *cmd = search_cmd(line->argv[1], global_cmd_root);
    for (int i = 2; i < line->argc; i++) {
        cmd->aliases = delete_alias(cmd->aliases, line->argv[i], cmd);
    }
}