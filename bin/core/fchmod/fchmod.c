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

_Static_assert(1, "bin/core/fchmod");
#include <fchmod_types.h>
#include <parse_modes.h>
#include <process_path.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        missing_argument("fchmod");
        return 1;
    }

    Options opts = {0};
    char *target = NULL;
    char *mode_raw = NULL;
    bool only_flag_seen = false;

    opts.only_file = true;
    opts.only_dir = true;
    opts.only_symlink = true;
    opts.recursive = true;

    for (int i = 1; i < argc; i++) {
        if (
            strcmp(argv[i], "--mode") == 0 &&
            i + 1 < argc
        ) {
            mode_raw = argv[++i];
        } else if (
            strcmp(argv[i], "--norecursive") == 0
        ) {
            opts.recursive = false;
        } else if (
            strcmp(argv[i], "--only") == 0 &&
            i + 1 < argc
        ) {
            if (!only_flag_seen) {
                opts.only_file = opts.only_dir =
                    opts.only_symlink =
                    false;
                only_flag_seen = true;
            }
            i++;
            if (
                strcmp(argv[i], "file") == 0
            ) {
                opts.only_file = true;
            } else if (
                strcmp(argv[i], "dir") == 0
            ) {
                opts.only_dir = true;
            } else if (
                strcmp(argv[i], "symlink") == 0
            ) {
                opts.only_symlink = true;
            } else {
                char invalid_args[256]; 
                snprintf(
                    invalid_args,
                    sizeof(invalid_args),
                    "--only %s",
                    argv[i]
                );

                invalid_option(
                    invalid_args,
                    "fchmod"
                );
                return 1;
            }
        } else if (
            strcmp(argv[i], "--ext") == 0 &&
            i + 1 < argc
        ) {
            strncpy(
                opts.ext,
                argv[++i],
                sizeof(opts.ext) - 1
            );
            opts.has_ext = true;
        } else if (target == NULL) {
            target = argv[i];
        }
    }

    if (!target || !mode_raw) {
        missing_argument("fchmod");
        return 1;
    }

    if (!parse_modes(mode_raw, &opts)) {
        printf(
            "%s[!] %sInvalid mode syntax: %s%s%s\n",
            color_R, color_N, color_GG, mode_raw, color_N
        );
        return 1;
    }

    char *target_copy = strdup(target);
    char *token = strtok(target_copy, ":");

    while (token != NULL) {
        if (strlen(token) > 0) {
            printf(
                "%s[*] %sProcessing: %s%s%s\n",
                color_B, color_N, color_GG, token, color_N
            );
            process_path(token, &opts);
        }
        token = strtok(NULL, ":");
    }

    free(target_copy);
    return 0;
}

// Copyright (c) 2026 Zeronetsec