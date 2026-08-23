// https://github.com/Zeronetsec/Ares

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>

typedef struct word_desc {
    char *word;
    int flags;
} WORD_DESC;

typedef struct word_list {
    struct word_list *next;
    WORD_DESC *word;
} WORD_LIST;

int catchypr_builtin(WORD_LIST *list);
char *get_string_value(const char *name);

int catchypr_builtin(WORD_LIST *list) {
    char *exit_str = get_string_value("exit_code");
    int exit_code = exit_str ?
        atoi(exit_str) :
        0;

    char *pwd = get_string_value("PWD");
    if (!pwd) pwd = "/";

    char pwd_copy[1024];
    strncpy(
        pwd_copy,
        pwd,
        sizeof(pwd_copy) - 1
    );
    pwd_copy[sizeof(pwd_copy) - 1] = '\0';

    char *bname = basename(pwd_copy);
    char cpath[1024];

    if (strcmp(pwd, "/") == 0) {
        snprintf(
            cpath,
            sizeof(cpath),
            "~/"
        );
    } else {
        snprintf(
            cpath,
            sizeof(cpath),
            "~/%s",
            bname
        );
    }

    char *path_color = "\001\033[0;33m\002";
    if (
        strcmp(cpath, "~/") == 0 ||
        strcmp(cpath, "~/root") == 0
    ) {
        path_color = "\001\033[0;31m\002";
    } else if (
        strcmp(cpath, "~/etc") == 0
    ) {
        path_color = "\001\033[0;32m\002";
    } else if (
        strcmp(cpath, "~/usr") == 0
    ) {
        path_color = "\001\033[0;34m\002";
    } else if (
        strcmp(cpath, "~/share") == 0
    ) {
        path_color = "\001\033[0;36m\002";
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    int hour = tm_info->tm_hour;

    char *symbol1 = "𖤍";
    switch (hour) {
        case 0: symbol1 = "☾"; break;
        case 1: symbol1 = "⋄"; break;
        case 5: symbol1 = "⚔"; break;
        case 7: symbol1 = "✈︎"; break;
        case 12: symbol1 = "𓃵"; break;
        case 20: symbol1 = "𖤐"; break;
    }

    int is_root = (geteuid() == 0);
    char *sss = is_root ?
        "#" :
        "$";

    char symbol2[128];
    char symbol3[128];

    if (exit_code == 0) {
        snprintf(
            symbol2,
            sizeof(symbol2),
            "\001\033[38;5;244m\002%s",
            symbol1
        );

        snprintf(
            symbol3,
            sizeof(symbol3),
            "\001\033[38;5;252m\002%s",
            sss
        );
    } else {
        snprintf(
            symbol2,
            sizeof(symbol2),
            "\001\033[1;34m\002%s",
            symbol1
        );

        snprintf(
            symbol3,
            sizeof(symbol3),
            "\001\033[1;31m\002%s",
            sss
        );
    }

    printf(
        "\001\033[?25h\002\n\001\033[38;5;244m\002┌──(\001\033[1;31m\002Ares\001\033[38;5;244m\002(%s\001\033[38;5;244m\002)\001\033[1;31m\002Framework\001\033[38;5;244m\002)-(%s%s\001\033[38;5;244m\002)\n\001\033[38;5;244m\002└──%s\001\033[0;32m\002 ", 
        symbol2, path_color, cpath, symbol3
    );

    return 0;
}

char *catchypr_doc[] = {
    "Dynamic PS1 generator.",
    (char *)NULL
};

struct builtin {
    char *name;
    int (*function)(WORD_LIST *);
    int flags;
    char **long_doc;
    char *short_doc;
    char *handle;
} catchypr_struct = {
    "catchypr",
    catchypr_builtin,
    1,
    catchypr_doc,
    "catchypr",
    0
};

// Copyright (c) 2026 Zeronetsec