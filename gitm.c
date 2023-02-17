#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include "dirent.h"
#include "stdbool.h"

#define max(a, b) ((a) > (b) ? (a) : (b))

char com_s[10];
int RoC, cur_int, commit_arr[10000] = {0};
FILE *argv_F = NULL;
DIR *zu_xian_d = NULL, *cur_d = NULL, *argv_d = NULL, *is_in_d = NULL;
struct dirent *zu_xian_p = NULL, *cur_p = NULL, *argv_p = NULL, *is_in_p = NULL;

void copy(char a[]);

void c_copy(char a[], char b[]);

bool is_change(char [], char f_name[][256], int a);

bool is_there(char f_name[][256], char d_name[]);

bool is_md5(char d_name[], char path[]);

int zu_xian(char argv[], char cur[], int zu_xian_num);

bool is_zu_xian(char argv[], int commit);

bool is_conflict(char cur[], char argv[], char zu_xian[]);

bool is_in_dir(char f_name[], char dir_name[]);

bool is_same(char name[], char dir1[], char dir2[]);

int main(int argc, char *argv[]) {
    int com = 0, a = 0, is_cur;
    char cur_s[10];
    char f_name[10][256], r_com[10000];
    DIR *dir;
    struct dirent *ptr;
    FILE *commit, *cur;

    if (argv[1])
        if (!strcmp(argv[1], "version"))
            fprintf(stderr, "%s",

#include "icon.txt"

            );

    if (!strcmp(argv[1], "init")) {
        mkdir(".gitm", 0777);
    }

    if (!strcmp(argv[1], "commit")) {
        int max = com;
        RoC = chdir(".gitm");
        dir = opendir(".");

        while ((ptr = readdir(dir)) != NULL) {
            if (ptr->d_name[0] != '.') {
                a++;
            }
        }

        if (a) {
            cur = fopen("cur", "r");
            RoC = fscanf(cur, "%s", cur_s);
            closedir(dir);
            RoC = chdir("commit");
            dir = opendir(".");
            fclose(cur);

            while ((ptr = readdir(dir)) != NULL) {
                if (ptr->d_name[0] != '.') {
                    sscanf(ptr->d_name, "%x", &com);
                    com = max(com, max);
                    max = com;
                }
            }
            com++;

            closedir(dir);
            dir = opendir(".");
            commit = fopen(cur_s, "a+");
            fprintf(commit, "%.8x ", com);

            sprintf(com_s, "%.8x", com);
            commit = fopen(com_s, "a+");
            fprintf(commit, "%.8x %s %.8x", com, cur_s, 0);
            RoC = chdir("..");
            cur = fopen("cur", "w+");
            fprintf(cur, "%.8x", com);
            mkdir(com_s, 0777);
            RoC = chdir("..");
        } else {
            com++;
            sprintf(com_s, "%.8x", com);
            mkdir(com_s, 0777);
            cur = fopen("cur", "w+");
            fputs(com_s, cur);
            mkdir("commit", 0777);
            RoC = chdir("commit");
            commit = fopen(com_s, "a+");
            fputs(com_s, commit);

            for (int i = 0; i < 2; ++i) {
                fprintf(commit, " %.8x", 0);
            }
            RoC = chdir("..");
            RoC = chdir("..");
        }
        closedir(dir);
        opendir(".");

        while ((ptr = readdir(dir)) != NULL) {
            if (ptr->d_name[0] != '.') {
                copy(ptr->d_name);
            }
        }
        a = 0;

        for (int i = 0; i < 10; ++i) {
            memset(f_name[i], '\0', 255);
        }
        printf("%.8x\n", com);
    }

    if (!strcmp(argv[1], "checkout")) {
        RoC = chdir(".gitm");
        cur = fopen("cur", "r+");
        RoC = fscanf(cur, "%s", cur_s);
        RoC = chdir("..");
//        printf("%s\n", cur_s);
        dir = opendir(".");

        while ((ptr = readdir(dir)) != NULL) {
            if (ptr->d_name[0] != '.') {
                strcpy(f_name[a], ptr->d_name);
                a++;
            }
        }

        if (argv[2][0] == '.') {
            a = 0;
            dir = opendir(".");

            while ((ptr = readdir(dir)) != NULL) {
                strcpy(f_name[a], ptr->d_name);
//                printf("%s\n", f_name[a]);

                if (f_name[a][0] != '.') {
                    remove(f_name[a]);
                }
                a++;
            }

            a = 0;
            RoC = chdir(".gitm");
            RoC = chdir(cur_s);
            dir = opendir(".");

            while ((ptr = readdir(dir)) != NULL) {
                strcpy(f_name[a], ptr->d_name);
//                printf("%s\n", f_name[a]);

                if (f_name[a][0] != '.') {
                    c_copy(f_name[a], cur_s);
                }
                a++;
            }
        } else {
            if (is_change(cur_s, f_name, a)) {
                a = 0;
                dir = opendir(".");

                while ((ptr = readdir(dir)) != NULL) {
                    if (f_name[a][0] != '.') {
                        remove(ptr->d_name);
                    }
                }
                a = 0;
                RoC = chdir(".gitm");
                cur = fopen("cur", "w+");
                fputs(argv[2], cur);
                RoC = chdir(argv[2]);
                dir = opendir(".");

                while ((ptr = readdir(dir)) != NULL) {
                    strcpy(f_name[a], ptr->d_name);
//                printf("%s\n", f_name[a]);

                    if (f_name[a][0] != '.') {
                        c_copy(f_name[a], argv[2]);
                    }
                    a++;
                }
            } else {
                printf("You've made change. Please commit or garbage your change.\n");
                return 1;
            }
        }
    }

    if (!strcmp(argv[1], "merge")) {
        int zu_xian_num;
        char zu_xian_s[10];
        RoC = chdir(".gitm");
        cur = fopen("cur", "r");
        RoC = fscanf(cur, "%s", cur_s);
        RoC = chdir("commit");
        zu_xian_num = zu_xian(argv[2], cur_s, 00000001);
        sprintf(zu_xian_s, "%.8x", zu_xian_num);
//        fprintf(stdout, "%s\n", zu_xian_s);
        RoC = chdir("..");

        cur_d = opendir(cur_s);
        argv_d = opendir(argv[2]);
        zu_xian_d = opendir(zu_xian_s);
//        printf("%s\n", cur_s);

        if (is_conflict(cur_s, argv[2], zu_xian_s)) {
            printf("conflict\n");
            return 1;
        } else {
//            printf("suc\n");
            RoC = chdir("..");
            dir = opendir(".");

            while ((ptr = readdir(dir)) != NULL) {
                if (f_name[a][0] != '.') {
                    remove(ptr->d_name);
                }
            }
            closedir(dir);
            int max = com;
            RoC = chdir(".gitm");
            RoC = chdir("commit");
            dir = opendir(".");

            while ((ptr = readdir(dir)) != NULL) {
                if (ptr->d_name[0] != '.') {
                    sscanf(ptr->d_name, "%x", &com);
                    com = max(com, max);
                    max = com;
                }
            }
            com++;
            closedir(dir);
            dir = opendir(".");
            commit = fopen(cur_s, "a+");
            fprintf(commit, " %.8x", com);

            fclose(commit);
            closedir(dir);
            dir = opendir(".");
            commit = fopen(argv[2], "a+");
            fprintf(commit, " %.8x", com);

            sprintf(com_s, "%.8x", com);
            commit = fopen(com_s, "a+");
            fprintf(commit, "%.8x %s %s", com, cur_s, argv[2]);
            RoC = chdir("..");
            fclose(cur);
            cur = fopen("cur", "w+");
            fprintf(cur, "%.8x", com);
            mkdir(com_s, 0777);

            closedir(cur_d);
            closedir(argv_d);
            closedir(zu_xian_d);
            cur_d = opendir(cur_s);
            argv_d = opendir(argv[2]);
            zu_xian_d = opendir(zu_xian_s);

            while ((cur_p = readdir(cur_d)) != NULL) {
                if (cur_p->d_name[0] != '.') {
                    if (is_in_dir(cur_p->d_name, zu_xian_s)) {
                        if (is_same(cur_p->d_name, cur_s, zu_xian_s)) {
                            RoC = chdir(cur_s);
                            c_copy(cur_p->d_name, cur_s);
                            RoC = chdir("..");
                        } else if (is_in_dir(cur_p->d_name, argv[2])) {
                            RoC = chdir(argv[2]);
                            c_copy(cur_p->d_name, cur_s);
                            RoC = chdir("..");
                        } else {
                            RoC = chdir(zu_xian_s);
                            c_copy(cur_p->d_name, cur_s);
                            RoC = chdir("..");
                        }
                    } else {
                        RoC = chdir(cur_s);
                        c_copy(cur_p->d_name, cur_s);
                        RoC = chdir("..");
                    }
                }
            }
            while ((argv_p = readdir(argv_d)) != NULL) {
                if (argv_p->d_name[0] != '.') {
                    if (!is_in_dir(argv_p->d_name, cur_s)) {
                        RoC = chdir(argv[2]);
                        c_copy(argv_p->d_name, cur_s);
                        RoC = chdir("..");
                    }
                }
            }
            RoC = chdir("..");
            closedir(dir);
            dir = opendir(".");
            while ((ptr = readdir(dir)) != NULL) {
                if (ptr->d_name[0] != '.') {
                    copy(ptr->d_name);
                }
            }
        }
    }
    return 0;
}

bool is_same(char name[], char dir1[], char dir2[]) {
    size_t a;
    FILE *p_file = NULL;
    char d[34], s[34], command[265];
    sprintf(command, "md5sum %s", name);
    RoC = chdir(dir1);
    p_file = popen(command, "r");
    a = fread(d, 1, 32, p_file);
//    printf("1\n%s\n", d);
    pclose(p_file);
    RoC = chdir("..");
    RoC = chdir(dir2);
    p_file = popen(command, "r");
    a = fread(s, 1, 32, p_file);
    RoC = chdir("..");
    pclose(p_file);
    d[32] = '\0';
    s[32] = '\0';
//    fprintf(stderr, "%s\n%s\n", d, s);
    return strcmp(d, s);
}

bool is_in_dir(char f_name[], char dir_name[]) {
    is_in_d = opendir(dir_name);
    while ((is_in_p = readdir(is_in_d)) != NULL) {
        if (is_in_p->d_name[0] != '.') {
            if (strcmp(is_in_p->d_name, f_name) == 0) {
//                fprintf(stderr, "dfs\n");
                closedir(is_in_d);
                return true;
            }
        }
    }
    closedir(is_in_d);
    return false;
}

bool is_conflict(char cur[], char argv[], char zu_xian[]) {
    while ((cur_p = readdir(cur_d)) != NULL) {
//        printf("fs");
        if (cur_p->d_name[0] != '.' && cur_p->d_name[0] != '\0' && cur_p->d_name[0] != '\n') {
            if (is_in_dir(cur_p->d_name, argv)) {
                if (is_in_dir(cur_p->d_name, zu_xian)) {
                    if (is_same(cur_p->d_name, cur, zu_xian) && is_same(cur_p->d_name, argv, zu_xian)) {
                        return true;
                    }
                } else {
                    return true;
                }
            }
        }
    }
//    printf("sed\n");
    closedir(zu_xian_d);
    zu_xian_d = opendir(zu_xian);

    while ((zu_xian_p = readdir(zu_xian_d)) != NULL) {
        if (zu_xian_p->d_name[0] != '.') {
            if (!is_in_dir(zu_xian_p->d_name, argv) && !is_in_dir(zu_xian_p->d_name, cur)) {
                return true;
            }
        }
    }
    return false;
}

bool is_zu_xian(char argv[], int commit) {
//    printf("dfe");
    FILE *argv_f = NULL;
    int i = 0, is_zu_xian_num = 0;
    char file_name[10], zi_file_name[10];
    sprintf(file_name, "%.8x", commit);
//    fprintf(stderr, "%s", file_name);
//    fprintf(stderr, "%s", argv);
    argv_f = fopen(file_name, "r");
    while (fscanf(argv_f, "%x", &is_zu_xian_num) != EOF) {
        if (i > 2) {
            sprintf(zi_file_name, "%.8x", is_zu_xian_num);
//            fprintf(stderr, "%s", zi_file_name);
//            fprintf(stderr, "%s", argv);
            if (!strcmp(zi_file_name, argv)) {
//                fprintf(stderr, "dvsd\n");
                fclose(argv_f);
                return true;
            } else {
                if (is_zu_xian(argv, is_zu_xian_num)) {
                    return true;
                }
            }
        }
        i++;
    }
    fclose(argv_f);
    return false;
}

int zu_xian(char argv[], char cur[], int zu_xian_num) {
    int i = 0;
    char file_name[10];
    sprintf(file_name, "%.8x", zu_xian_num);
    argv_F = fopen(file_name, "r");
    while (fscanf(argv_F, "%x", commit_arr + i) != EOF) i++;
    fclose(argv_F);

    for (int j = 3; j < i; ++j) {
//        printf("dfsed\n");
        if (is_zu_xian(cur, commit_arr[j]) && is_zu_xian(argv, commit_arr[j])) {
            i = commit_arr[j];
            for (int k = 0; k <= i; ++k) {
                commit_arr[k] = 0;
            }
            return zu_xian(argv, cur, i);
        }
    }
    return zu_xian_num;
}

bool is_md5(char d_name[], char path[]) {
    size_t a;
    FILE *p_file = NULL;
    char d[34], s[34], command[265];
    sprintf(command, "md5sum %s", d_name);
    p_file = popen(command, "r");
    a = fread(d, 1, 33, p_file);
//    printf("1\n%s\n", d);
    pclose(p_file);
    RoC = chdir("..");
    RoC = chdir("..");
    p_file = popen(command, "r");
    a = fread(s, 1, 33, p_file);
    RoC = chdir(".gitm");
    RoC = chdir(path);
    return !strcmp(d, s);
}

bool is_there(char f_name[][256], char d_name[]) {
    int i = 0, j = 0;
    while (f_name[i] != NULL && f_name[i][0] != '\0') {
        j += !strcmp(f_name[i], d_name);
        i++;
    }
    return j;
}

bool is_change(char cur[], char f_name[][256], int a) {
    int b = 0;
    DIR *dir;
    struct dirent *ptr;
    RoC = chdir(".gitm");
    RoC = chdir(cur);
    dir = opendir(".");

    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_name[0] != '.') {
            b++;
        }
    }

    closedir(dir);
    dir = opendir(".");

    if (a != b)
        return 0;
//    printf("%d %d\n", a, b);

    b = 0;

    while ((ptr = readdir(dir)) != NULL) {
        if (ptr->d_name[0] != '.') {
            if (is_there(f_name, ptr->d_name)) {
                b = b + is_md5(ptr->d_name, cur);
            }
        }
    }

    RoC = chdir("..");
    RoC = chdir("..");
//    printf("%d\n", a);

    if (a == b)
        return 1;
    else
        return 0;
}

void c_copy(char a[], char b[]) {
    FILE *fs, *fd;
    int c;
    fs = fopen(a, "a+");

    RoC = chdir("..");
    RoC = chdir("..");
    fd = fopen(a, "a+");
    RoC = chdir(".gitm");
    RoC = chdir(b);

    while ((c = fgetc(fs)) != EOF) {
        fputc(c, fd);
    }
    fclose(fs);
    fclose(fd);
}

void copy(char a[]) {
    FILE *fs, *fd;
    int c;
    fs = fopen(a, "a+");

    RoC = chdir(".gitm");
    RoC = chdir(com_s);
    fd = fopen(a, "a+");
    RoC = chdir("..");
    RoC = chdir("..");
//    fprintf(stderr, "vddv\n");

    while ((c = fgetc(fs)) != EOF) {
        fputc(c, fd);
//        fprintf(stderr, "%c", c);
    }
    fclose(fs);
    fclose(fd);
}