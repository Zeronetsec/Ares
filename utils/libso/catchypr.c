// https://github.com/Zeronetsec/Ares

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <builtins.h>
#include <shell.h>

int catchypr_builtin(WORD_LIST *list);

int catchypr_builtin(WORD_LIST *list) {
    char *exit_str = get_string_value("exit_code");
    int exit_code = exit_str ?
        atoi(exit_str) :
        0;

    char *pwd = get_string_value("PWD");
    if (!pwd) pwd = "/";

    char *home = get_string_value("HOME");

    char *dirtrim_str = get_string_value("PROMPT_DIRTRIM");
    int dirtrim = dirtrim_str ?
        atoi(dirtrim_str) :
        0;

    char working_path[2048];
    int is_home = 0;
    size_t home_len = home ?
        strlen(home) :
        0;

    if (
        home &&
        strncmp(pwd, home, home_len) == 0 && 
       (
            pwd[home_len] == '/' ||
            pwd[home_len] == '\0'
        )
    ) {
        snprintf(
            working_path,
            sizeof(working_path),
            "~%s",
            pwd + home_len
        );
        is_home = 1;
    } else {
        snprintf(
            working_path,
            sizeof(working_path),
            "%s",
            pwd
        );
    }

    char cpath[2048];
    if (dirtrim > 0) {
        int slashes_found = 0;
        int len = strlen(working_path);
        int i = len - 1;

        if (i > 0 && working_path[i] == '/') i--;
        for (; i >= 0; i--) {
            if (working_path[i] == '/') {
                slashes_found++;
                if (slashes_found == dirtrim) break;
            }
        }

        int prefix_len = is_home ? 1 : 0;
        if (i > prefix_len) {
            if (is_home) {
                snprintf(
                    cpath,
                    sizeof(cpath),
                    "~/...%s",
                    &working_path[i]
                );
            } else {
                snprintf(
                    cpath,
                    sizeof(cpath),
                    "/...%s",
                    &working_path[i]
                );
            }
        } else {
            strncpy(
                cpath,
                working_path,
                sizeof(cpath)
            );
        }
    } else {
        strncpy(
            cpath,
            working_path,
            sizeof(cpath)
        );
    }

    char *path_color = "\001\x1b[0;33m\002";
    if (
        strcmp(pwd, "/") == 0 ||
        strncmp(pwd, "/root", 5) == 0
    ) {
        path_color = "\001\x1b[0;31m\002";
    } else if (
        strncmp(pwd, "/etc", 4) == 0
    ) {
        path_color = "\001\x1b[0;32m\002";
    } else if (
        strncmp(pwd, "/usr", 4) == 0
    ) {
        path_color = "\001\x1b[0;34m\002";
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char *symbol1 = "𖤍";
    switch (tm_info->tm_hour) {
        case 0: symbol1 = "☾"; break;
        case 1: symbol1 = "⋄"; break;
        case 5: symbol1 = "⚔"; break;
        case 7: symbol1 = "✈︎"; break;
        case 12: symbol1 = "𓃵"; break;
        case 20: symbol1 = "𖤐"; break;
    }

    char *sss = (geteuid() == 0) ?
        "#" :
        "$";

    char symbol2[128];
    char symbol3[128];

    if (exit_code == 0) {
        snprintf(
            symbol2,
            sizeof(symbol2),
            "\001\x1b[38;5;244m\002%s",
            symbol1
        );

        snprintf(
            symbol3,
            sizeof(symbol3),
            "\001\x1b[38;5;252m\002%s",
            sss
        );
    } else {
        snprintf(
            symbol2,
            sizeof(symbol2),
            "\001\x1b[1;34m\002%s",
            symbol1
        );

        snprintf(
            symbol3,
            sizeof(symbol3),
            "\001\x1b[1;31m\002%s",
            sss
        );
    }

    printf(
        "\001\x1b[?25h\002\n"
        "\001\x1b[38;5;244m\002┌──(\001\x1b[1;31m\002Ares\001\x1b[38;5;244m\002(%s\001\x1b[38;5;244m\002)\001\x1b[1;31m\002Framework\001\x1b[38;5;244m\002)-(%s%s\001\x1b[38;5;244m\002)\n"
        "\001\x1b[38;5;244m\002└──%s\001\x1b[0;32m\002 ", 
        symbol2, path_color, cpath, symbol3
    );

    return 0;
}

char *catchypr_doc[] = {
    "Dynamic PS1 generator.",
    (char *)NULL
};

struct builtin catchypr_struct = {
    "catchypr",
    catchypr_builtin,
    1,
    catchypr_doc,
    "catchypr",
    0
};

// Copyright (c) 2026 Zeronetsec