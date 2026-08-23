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
#include <builtins/bashgetopt.h>
#include <builtins/common.h>

extern int unset_builtin(WORD_LIST *);

int destroyv_builtin(WORD_LIST *list) {
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

    char *data_copy = strdup(input_data);
    if (!data_copy) {
        return EXECUTION_FAILURE;
    }

    char *saveptr;
    int inside_bracket = 0;

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

            WORD_LIST *wl_name = make_word_list(
                make_word(line),
                NULL
            );

            unset_builtin(wl_name);
            dispose_words(wl_name);
        }

        next_line:
            line = strtok_r(
                NULL,
                "\n",
                &saveptr
            );
    }

    free(data_copy);
    return EXECUTION_SUCCESS;
}

char *destroyv_doc[] = {
    "Destroy shell variables.",
    (char *)NULL
};

struct builtin destroyv_struct = {
    "destroyv",
    destroyv_builtin,
    BUILTIN_ENABLED,
    destroyv_doc,
    "destroyv : \"( ... )\"",
    0
};

// Copyright (c) 2026 Zeronetsec