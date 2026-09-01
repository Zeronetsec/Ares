// https://github.com/Zeronetsec/Ares

#ifndef PARSE_MODES_H
#define PARSE_MODES_H

_Static_assert(1, "system");
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

_Static_assert(1, "bin/core/fchmod");
#include <fchmod_types.h>

static inline bool parse_modes(
    const char *mode_str,
    Options *opts
) {
    char *buf = strdup(mode_str);
    if (!buf) return false;

    char *token = strtok(buf, ":");
    while (token != NULL) {
        char type = token[0];
        char *mode_val_str = token + 1;

        if (strlen(token) < 2) {
            free(buf);
            return false;
        }

        mode_t parsed_mode = strtol(
            mode_val_str,
            NULL,
            8
        );

        if (
            type == 'd' ||
            type == 'D'
        ) {
            opts->mode_dir = parsed_mode;
            opts->mode_dir_set = true;
        } else if (
            type == 'f' ||
            type == 'F'
        ) {
            opts->mode_file = parsed_mode;
            opts->mode_file_set = true;
        } else if (
            type == 's' ||
            type == 'S'
        ) {
            opts->mode_symlink = parsed_mode;
            opts->mode_symlink_set = true;
        } else {
            mode_t global_mode = strtol(token, NULL, 8);
            opts->mode_dir = global_mode;
            opts->mode_file = global_mode;
            opts->mode_symlink = global_mode;
            opts->mode_dir_set = opts->mode_file_set =
                opts->mode_symlink_set =
                true;
        }
        token = strtok(NULL, ":");
    }
    free(buf);

    if (
        opts->mode_file_set &&
        !opts->mode_symlink_set
    ) {
        opts->mode_symlink = opts->mode_file;
        opts->mode_symlink_set = true;
    }

    return opts->mode_dir_set ||
        opts->mode_file_set ||
        opts->mode_symlink_set;
}

#endif

// Copyright (c) 2026 Zeronetsec