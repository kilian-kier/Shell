#include "../include/functions.h"

char *pwd() {
    int size = PATH_MAX;
    char *buf = (char *) malloc(size);
    return getcwd(buf, size);
}

int file_size(char *filename) {
    struct stat st;
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
}

char *last_change(char *filename) {
    struct stat st;
    struct tm *t;
    char *buf = malloc(100 * sizeof(char));
    if (stat(filename, &st) == 0) {
        t = localtime(&(st.st_mtime));
        strftime(buf, 100, "%d.%m.%Y %H:%M", t);
        return buf;
    }
}

int copy_file(const char *old_file, const char *new_file) {
    FILE *old = fopen(old_file, "rb");
    FILE *new = fopen(new_file, "r");
    if (new != NULL) {
        puts("Datei bereits vorhanden, soll sie überschrieben werden [j/n]");
        int ret;
        do {
            fflush(stdin);
            ret = getchar();
            if (ret == 'n')
                return -1;
            else if (ret != 'j')
                puts("Bitte entweder j oder n eingeben");
        } while (ret != 'j');
    }
    fclose(new);
    new = fopen(new_file, "wb");

    int ch;
    if (old != NULL && new != NULL) {
        while ((ch = fgetc(old)) != EOF)
            fputc(ch, new);
        fclose(old);
        fclose(new);
    } else {
        perror("copy_file");
        return 1;
    }
    return 0;
}

char *destination_path(const char *dir, const char *name) {
    char *directory = malloc(strlen(dir) * sizeof(char));
    directory = strcpy(directory, dir);
    char *l1 = (strrchr(name, path_separator));
    char *ret;
    if (l1 == NULL) {
        char separator[2] = {path_separator, 0};
        ret = strcat(directory, strcat(separator, name));
        return ret;
    } else {
        ret = strcat(directory, l1);
        return ret;
    }
}

int convertChmod(int x) {
    int ret = 0;
    int values[] = {400, 200, 100, 40, 20, 10, 4, 2, 1};
    int dez[] = {256, 128, 64, 32, 16, 8, 4, 2, 1};
    for (short i = 0; i < 9; i++) {
        if (x - values[i] >= 0) {
            x = x - values[i];
            ret = ret + dez[i];
        }
    }

    if (x == 0) {
        return ret;
    } else {
        return -1;
    }
}

#ifdef WIN

char *whoami() {
    char *user = getenv("USERNAME");
    if (user == NULL)
        return "NO_USERNAME";
    return user;
}

int file_blksize(char *filename) {
    struct stat st;
    int ret = 0;
    if (stat(filename, &st) == 0) {
        while (ret < st.st_size) {
            ret += 4096;
        }
        return ret;
    }
}

char *file_owner(char *filename) {
    struct stat st;
    char *buf = malloc(10 * sizeof(char));
    if (stat(filename, &st) == 0) {
        return itoa(st.st_uid, buf, 10);
    }
}

char *file_group(char *filename) {
    struct stat st;
    char *buf = malloc(10 * sizeof(char));
    if (stat(filename, &st) == 0) {
        return itoa(st.st_gid, buf, 10);
    }
}

#else
char *whoami() {
    char *user = getlogin();
    if (user == NULL)
        return "NO_USERNAME";
    return user;
}
char *file_permission(char *filename) {
    struct stat st;
    char *mode = malloc(12 * sizeof(char));
    if (stat(filename, &st) == 0) {
        mode_t perm = st.st_mode;
        mode[0] = (perm & S_IRUSR) ? 'r' : '-';
        mode[1] = (perm & S_IWUSR) ? 'w' : '-';
        mode[2] = (perm & S_IXUSR) ? 'x' : '-';
        mode[3] = ' ';
        mode[4] = (perm & S_IRGRP) ? 'r' : '-';
        mode[5] = (perm & S_IWGRP) ? 'w' : '-';
        mode[6] = (perm & S_IXGRP) ? 'x' : '-';
        mode[7] = ' ';
        mode[8] = (perm & S_IROTH) ? 'r' : '-';
        mode[9] = (perm & S_IWOTH) ? 'w' : '-';
        mode[10] = (perm & S_IXOTH) ? 'x' : '-';
        mode[11] = '\0';
        return mode;
    }
}

int file_blksize(char *filename) {
    struct stat st;
    int ret = 0;
    if (stat(filename, &st) == 0) {
        while (ret < st.st_size) {
            ret += st.st_blksize;
        }
        return ret;
    }
}

char *file_owner(char *filename) {
    struct stat st;
    struct passwd *pw;
    if (stat(filename, &st) == 0) {
        pw = getpwuid(st.st_uid);
        return pw->pw_name;
    }
}

char *file_group(char *filename) {
    struct stat st;
    struct group *g;
    if (stat(filename, &st) == 0) {
        g = getgrgid(st.st_gid);
        return g->gr_name;
    }
}
#endif