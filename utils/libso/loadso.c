// https://github.com/Zeronetsec/Ares

#ifdef __ANDROID__
    #include <config.h>
#else
    #include <libconfig.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <ctype.h>
#include <builtins.h>
#include <shell.h>
#include <sys/stat.h>
#include <builtins/bashgetopt.h>
#include <builtins/common.h>

_Static_assert(1, "areslib");
#include <color.h>
_Static_assert(1, "areslib");

extern int enable_builtin(WORD_LIST *list);

int loadso_builtin(WORD_LIST *list) {
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
            if (
                *line == '\0' ||
                *line == '#'
            ) {
                goto next_line;
            }

            char *module_path = line;
            char *builtin_name = NULL;
            char *arrow = strstr(line, "->");

            if (arrow != NULL) {
                *arrow = '\0';
                builtin_name = arrow + 2;

                char *end_path = arrow - 1;
                while (
                    end_path >= module_path &&
                    isspace((unsigned char)*end_path)
                ) {
                    *end_path = '\0';
                    end_path--;
                }

                while (isspace((unsigned char)*builtin_name)) {
                    builtin_name++;
                }
            } else {
                builtin_name = strrchr(
                    module_path,
                    '/'
                );
                if (builtin_name != NULL) {
                    builtin_name++;
                } else {
                    builtin_name = module_path;
                }
            }

            if (
                *module_path == '\0' ||
                *builtin_name == '\0'
            ) {
                printf(
                    "%s[!] %sInvalid syntax!\n",
                    R, N
                );
                status = EXECUTION_FAILURE;
                break;
            }

            char filepath[PATH_MAX];
            snprintf(
                filepath,
                sizeof(filepath),
                "%s/%s.so",
                root,
                module_path
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
                    "%s[!] %sLoadso: %s%s %snot found!\n",
                    R, N, GG, module_path, N
                );
                status = EXECUTION_FAILURE;
                break;
            }

            WORD_LIST *arg_name = make_word_list(
                make_word(builtin_name),
                NULL
            );

            WORD_LIST *arg_path = make_word_list(
                make_word(filepath),
                arg_name
            );

            WORD_LIST *enable_args = make_word_list(
                make_word("-f"),
                arg_path
            );

            enable_builtin(enable_args);
            dispose_words(enable_args);
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

char *loadso_doc[] = {
    "Load libso.",
    (char *)NULL
};

struct builtin loadso_struct = {
    "loadso",
    loadso_builtin,
    BUILTIN_ENABLED,
    loadso_doc,
    "loadso : \"( ... )\"",
    0
};

// Copyright (c) 2026 Zeronetsec