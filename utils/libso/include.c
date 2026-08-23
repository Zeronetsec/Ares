// https://github.com/Zeronetsec/Ares

#ifdef __ANDROID__
    #include <config.h>
#else
    #include <libconfig.h>
#endif

#include <color.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <ctype.h>
#include <builtins.h>
#include <shell.h>
#include <builtins/bashgetopt.h>
#include <builtins/common.h>

extern int source_builtin(WORD_LIST *list);

int include_builtin(WORD_LIST *list) {
    if (
        !list ||
        !list->word ||
        !list->word->word ||
        strcmp(list->word->word, ":") != 0
    ) {
        return EXECUTION_FAILURE;
    }

    list = list->next;
    if (
        !list ||
        !list->word ||
        !list->word->word
    ) {
        return EXECUTION_FAILURE;
    }

    char *input_data = list->word->word;
    if (input_data[0] == '\0') {
        return EXECUTION_FAILURE;
    }

    SHELL_VAR *root_var = find_variable("__aresroot__");
    char *root = root_var ?
        value_cell(root_var) :
        ".";

    char *data_copy = strdup(input_data);
    if (!data_copy) {
        return EXECUTION_FAILURE;
    }

    char *saveptr;
    int inside_bracket = 0;
    int status = EXECUTION_SUCCESS;

    char *line = strtok_r(
        data_copy,
        "\n",
        &saveptr
    );

    while (line != NULL) {
        while (isspace((unsigned char)*line)) line++;
        char *end = line + strlen(line) - 1;
        while (
            end >= line &&
            isspace((unsigned char)*end)
        ) {
            *end = '\0';
            end--;
        }

        if (strchr(line, '(') != NULL) {
            inside_bracket = 1;
            goto next_line;
        }

        if (strchr(line, ')') != NULL) {
            inside_bracket = 0;
            goto next_line;
        }

        if (inside_bracket) {
            if (*line == '\0' || *line == '#') {
                goto next_line;
            }

            char filepath[PATH_MAX];
            snprintf(
                filepath,
                sizeof(filepath),
                "%s/%s.sh",
                root,
                line
            );

            struct stat st;
            if (
                stat(
                    filepath,
                    &st
                ) != 0 ||
                !S_ISREG(st.st_mode)
            ) {
                printf(
                    "%s[!] %sInclude: %s%s %snot found!\n",
                    R, N, GG, line, N
                );
                status = EXECUTION_FAILURE;
                break;
            }

            WORD_LIST *source_args = make_word_list(
                make_word(
                    filepath
                ),
                NULL
            );
            source_builtin(source_args);
            dispose_words(source_args);
        }

        next_line:
            line = strtok_r(
                NULL,
                "\n",
                &saveptr
            );
    }

    free(data_copy);
    return status;
}

char *include_doc[] = {
    "Include shell modules.",
    (char *)NULL
};

struct builtin include_struct = {
    "include",
    include_builtin,
    BUILTIN_ENABLED,
    include_doc,
    "include : \"( ... )\"",
    0
};

// Copyright (c) 2026 Zeronetsec