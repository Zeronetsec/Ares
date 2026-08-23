// https://github.com/Zeronetsec/Ares

#ifdef __ANDROID__
    #include <config.h>
#else
    #include <libconfig.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <builtins.h>
#include <shell.h>
#include <variables.h>

extern void builtin_error(const char *format, ...);

int unreadonlyv_builtin(WORD_LIST *list) {
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
        !list->word->word ||
        list->word->word[0] == '\0'
    ) {
        return EXECUTION_FAILURE;
    }

    char *input_data = list->word->word;
    char *data_copy = strdup(input_data);
    if (!data_copy) {
        return EXECUTION_FAILURE;
    }

    char *saveptr;
    int inside_bracket = 0;
    int final_status = EXECUTION_SUCCESS;

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

        if (
            inside_bracket &&
            *line != '\0' &&
            *line != '#'
        ) {
            char *var_name = line;
            SHELL_VAR *var = find_variable(var_name);

            if (var) {
                VUNSETATTR(
                    var,
                    att_readonly
                );
            } else {
                builtin_error(
                    "%s: variable not found!",
                    var_name
                );
                final_status = EXECUTION_FAILURE;
            }
        }

    next_line:
        line = strtok_r(
            NULL,
            "\n",
            &saveptr
        );
    }

    free(data_copy);
    return final_status;
}

char *unreadonlyv_doc[] = {
    "Removes readonly attribute from variables inside block.",
    (char *)NULL
};

struct builtin unreadonlyv_struct = {
    "unreadonlyv",
    unreadonlyv_builtin,
    BUILTIN_ENABLED,
    unreadonlyv_doc,
    "unreadonlyv : \"( ... )\"",
    0
};

// Copyright (c) 2026 Zeronetsec