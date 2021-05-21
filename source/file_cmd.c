#include "../include/file_cmd.h"

void cat(struct cmd_line *line) {
    if (line->argc < 2) {
        fprintf(stderr, "%s\n", "Bitte übergib min. 1 Parameter");
        return;
    }
    for (int i = 1; i < line->argc; i++) {
        FILE *f = fopen(line->argv[i], "rb");
        if (f == NULL) {
            perror("fopen");
            return;
        }
        int c = 0;
        while (1) {
            c = getc(f);
            if (c == EOF)
                break;
            printf("%c", c);
        }
        printf("\n");
        fclose(f);
    }
}

void touch(struct cmd_line *line) {
    if (line->argc < 2) {
        fprintf(stderr, "%s\n", "Bitte übergib min. 1 Parameter");
        return;
    }
    for (int i = 1; i < line->argc; i++) {
        FILE *f = fopen(line->argv[i], "w");
        fclose(f);
    }
}

void rm(struct cmd_line *line) {
    static short del_dir = 0;

    if (line->argc < 2) {
        fprintf(stderr, "%s\n", "Bitte übergib min. 1 Parameter");
        return;
    }
    for (int i = 1; i < line->argc; i++) {
        DIR *d = opendir(line->argv[1]);
        if (d == NULL) {
            del_dir = 0;
            if (remove(line->argv[i]) != 0) {
                perror("remove failed");
            }
        } else {
            if (del_dir == 0) {
                puts("Willst du den Ordner und dessen Unterordner und Dateien löschen?[j/n]");
                do {
                    fflush(stdin);
                    switch (getchar()) {
                        case 'j':
                            del_dir = 1;
                            break;
                        case 'n':
                            closedir(d);
                            return;
                        default:
                            del_dir = -1;
                            break;
                    }
                } while (del_dir == -1);
            }
            struct dirent *files;
            chdir(line->argv[1]);
            while ((files = readdir(d)) != NULL) {
                if (strcmp(files->d_name, ".") == 0 || strcmp(files->d_name, "..") == 0)
                    continue;
                char *ptr[] = {"rm", files->d_name};
                rm(create_line(2, 3 + (int) strlen(files->d_name), ptr));
            }
            closedir(d);
            chdir("..");
            rmdir(line->argv[i]);
        }
    }
    del_dir = 0;
}

void mv(struct cmd_line *line) {
    if (line->argc < 3) {
        fprintf(stderr, "%s\n", "Bitte übergib min. 2 Parameter");
        return;
    }
    for (int i = 2; i < line->argc; i++) {
        char *temp_destination = malloc(strlen(line->argv[1]) * sizeof(char));
        strcpy(temp_destination, line->argv[1]);
        char *temp_source = malloc(strlen(line->argv[i]) * sizeof(char));
        strcpy(temp_source, line->argv[i]);
        if (copy_file(temp_source, destination_path(temp_destination, temp_source)) != 0)
            return;
        remove(line->argv[i]);
        temp_destination = NULL;
        temp_source = NULL;
        free(temp_destination);
        free(temp_source);
    }
}

void cp(struct cmd_line *line) {
    if (line->argc < 3) {
        fprintf(stderr, "%s\n", "Bitte übergib min. 2 Parameter");
        return;
    }
    DIR *d = opendir(line->argv[1]);
    if (d == NULL) {
        FILE *f = fopen(line->argv[1], "r");
        if (f != NULL) {
            puts("Datei bereits vorhanden, soll sie überschrieben werden [j/n]");
            int ret;
            do {
                fflush(stdin);
                ret = getchar();
                if (ret == 'n')
                    return;
                else if (ret != 'j')
                    puts("Bitte entweder j oder n eingeben");
            } while (ret != 'j');
        }
        fclose(f);
        FILE *dest = fopen(line->argv[1], "w");
        for (int i = 2; i < line->argc; i++) {
            FILE *source = fopen(line->argv[i], "r");
            int ch;
            if (dest != NULL && source != NULL) {
                while ((ch = fgetc(source)) != EOF)
                    fputc(ch, dest);
                fclose(source);
                if (i != line->argc - 1)
                    fputc('\n', dest);
            } else {
                perror("copy_file");
                return;
            }
        }
        fclose(dest);
        return;
    }
    for (int i = 2; i < line->argc; i++) {
        char *temp_destination = malloc(strlen(line->argv[1]) * sizeof(char));
        strcpy(temp_destination, line->argv[1]);
        char *temp_source = malloc(strlen(line->argv[i]) * sizeof(char));
        strcpy(temp_source, line->argv[i]);
        if (copy_file(temp_source, destination_path(temp_destination, temp_source)) != 0)
            return;
        temp_destination = NULL;
        temp_source = NULL;
        free(temp_destination);
        free(temp_source);
    }
}

void chmode(struct cmd_line *line) {
    if (line->argc < 3) {
        fprintf(stderr, "%s\n", "Bitte übergib min. 2 Parameter");
        return;
    }
    char *ptr;

    int temp = strtol(line->argv[1], &ptr, 10);
    if (strcmp(ptr, "") != 0) {
        fprintf(stderr, "%s\n", "Der letzte Parameter muss ein numerischer Code sein");
        return;
    }
    int chmod_code = convertChmod(temp);
    FILE *f;
    if (chmod_code != -1) {
        for (int i = 2; i < line->argc; i++) {
            f = fopen(line->argv[i], "r");
            if (f == NULL) {
                fprintf(stderr, "Die Datei %s gibt es nicht!\n", line->argv[i]);
                continue;
            }
            fclose(f);
            chmod(line->argv[i], chmod_code);
        }
    } else
        fprintf(stderr, "%s\n", "Falscher chmod code");
}

void write_to_file(struct cmd_line *line) {
    if (line->argc < 3) {
        fprintf(stderr, "%s\n", "Bitte übergib min. 2 Parameter");
        return;
    }
    int i = 1;
    FILE *f;
    if (strcmp(line->argv[i], "-a") == 0) {
        i++;
        f = fopen(line->argv[i], "a");
        fseek(f, 0, SEEK_END);
        int pos = ftell(f);
        rewind(f);
        int start = ftell(f);
        fseek(f, pos, SEEK_SET);
        if (pos > start)
            fputc('\n', f);
    } else
        f = fopen(line->argv[i], "w");

    for (i = i + 1; i < line->argc; i++) {
        fwrite(line->argv[i], sizeof(char), strlen(line->argv[i]), f);
        fputc(' ', f);
    }
    fclose(f);
}