// https://github.com/Zeronetsec/Ares

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

_Static_assert(1, "lib/c");
#include <color.h>
#include <missing_argument.h>

_Static_assert(1, "bin/core/acon");
#include <acon_types.h>
#include <load_syntax.h>
#include <run_acon.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        missing_argument("acon");
        return 1;
    }

    char *acon_file = NULL;
    char *syntax_file = NULL;
    int raw_mode = 0;

    for (int i = 1; i < argc; i++) {
        if (
            strcmp(argv[i], "--map") == 0
            && i + 1 < argc
        ) {
            syntax_file = argv[++i];
        } else if (
            strcmp(argv[i], "--raw") == 0
            && i + 1 < argc
        ) {
            i++;
            if (
                strcmp(argv[i], "full") == 0
            ) {
                raw_mode = 1;
            } else if (
                strcmp(argv[i], "escape") == 0
            ) {
                raw_mode = 2;
            }
        } else if (
            argv[i][0] != '-'
        ) {
            acon_file = argv[i];
        }
    }

    if (!acon_file) {
        missing_argument("acon");
        return 1;
    }

    char default_map[1024];
    if (!syntax_file) {
        char *config_dir = getenv("__config__");
        if (config_dir) {
            snprintf(
                default_map,
                sizeof(default_map),
                "%s/acon/acon.syntax",
                config_dir
            );
            syntax_file = default_map;
        } else {
            fprintf(
                stderr,
                "%s[!] %sDefault syntax map is empty!\n",
                color_R, color_N
            );
            return 1;
        }
    }

    if (raw_mode != 1) {
        load_syntax(syntax_file);
    }

    run_acon(acon_file, raw_mode);
    return 0;
}

// Copyright (c) 2026 Zeronetsec