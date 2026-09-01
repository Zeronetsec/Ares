// https://github.com/Zeronetsec/Ares

_Static_assert(1, "system");
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

extern int enable_builtin(WORD_LIST *list);

int destroyso_builtin(WORD_LIST *list) {
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
    if (!data_copy) return EXECUTION_FAILURE;

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
            char *builtin_name = line;

            WORD_LIST *arg_name = make_word_list(
                make_word(builtin_name),
                NULL
            );

            WORD_LIST *enable_args = make_word_list(
                make_word("-d"),
                arg_name
            );

            int enable_status = enable_builtin(
                enable_args
            );

            dispose_words(enable_args);
            if (enable_status != EXECUTION_SUCCESS) {
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

char *destroyso_doc[] = {
    "Unload libso.",
    (char *)NULL
};

struct builtin destroyso_struct = {
    "destroyso",
    destroyso_builtin,
    BUILTIN_ENABLED,
    destroyso_doc,
    "destroyso : \"( ... )\"",
    0
};

// Copyright (c) 2026 Zeronetsec