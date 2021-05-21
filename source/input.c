#include "../include/input.h"

void input(FILE *history) {
    char *buffer = malloc(MAX_SIZE * sizeof(char));
    fflush(stdin);
    fgets(buffer, MAX_SIZE, stdin);
    if (strcmp(buffer, "\n") == 0)
        return;
    buffer = realloc(buffer, strlen(buffer) * sizeof(char));
    fputs(buffer, history);
    cmd_line *line = malloc(sizeof(cmd_line));
    line->argc = 0;
    line->argv = (char **) malloc(MAX_SIZE * sizeof(char));
    line->argv[line->argc] = strtok(buffer, " \n");
    while (line->argv[line->argc] != NULL) {
        line->argc++;
        line->argv[line->argc] = strtok(NULL, " \n");
    }

    command *temp = search_cmd(line->argv[0], global_cmd_root);
    if (temp != NULL)
        temp->cmd_functions(line);
    else {
        printf("Diesen Befehl gibt es nicht, soll er im System gesucht und ausgeführt werden [j/n]\n");
        int ret;
        do {
            fflush(stdin);
            ret = getchar();
            switch (ret) {
                case 'n':
                    break;
                case 'j':
                    system(buffer);
                    break;
                default:
                    printf("Bitte gib entweder j oder n ein\n");
            }
        } while (ret != 'j' && ret != 'n');
    }
    free(buffer);
    free(line->argv);
    free(line);
    printf("\n");
}
