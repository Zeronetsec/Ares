// https://github.com/Zeronetsec/Ares

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

_Static_assert(1, "lib/c");
#include <color.h>
#include <missing_argument.h>
#include <invalid_option.h>

_Static_assert(1, "bin/core/rmext");
#include <process_dir.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        missing_argument("rmext");
        return 1;
    }

    const char *target_path = NULL;
    bool recursive = true;
    char *ext_str = NULL;

    for (int i = 1; i < argc; i++) {
        if (
            strcmp(argv[i], "--no-recursive") == 0
        ) {
            recursive = false;
        } else if (
            strcmp(argv[i], "--ext") == 0
        ) {
            if (i + 1 < argc) {
                ext_str = argv[++i];
            } else {
                missing_argument("rmext");
                return 1;
            }
        } else if (target_path == NULL) {
            target_path = argv[i];
        } else {
            invalid_option(argv[i], "rmext");
            return 1;
        }
    }

    if (!target_path) {
        missing_argument("rmext");
        return 1;
    }

    char **exts = NULL;
    int *ext_lens = NULL;
    int ext_count = 0;

    if (ext_str) {
        int commas = 0;
        for (int i = 0; ext_str[i]; i++) {
            if (ext_str[i] == ',') {
                commas++;
            }
        }

        exts = malloc((commas + 2) * sizeof(char*));
        ext_lens = malloc((commas + 2) * sizeof(int));

        char *token = strtok(ext_str, ",");
        while (token) {
            while (*token == ' ') token++;

            char *end = token + strlen(token) - 1;
            while (end > token && *end == ' ') {
                *end = '\0';
                end--;
            }

            if (strlen(token) > 0) {
                exts[ext_count] = token;
                ext_lens[ext_count] = strlen(token);
                ext_count++;
            }
            token = strtok(NULL, ",");
        }
    }

    process_dir(
        target_path,
        recursive,
        exts,
        ext_lens,
        ext_count
    );

    if (exts) free(exts);
    if (ext_lens) free(ext_lens);

    return 0;
}

// Copyright (c) 2026 Zeronetsec