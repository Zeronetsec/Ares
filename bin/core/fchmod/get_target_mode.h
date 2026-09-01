// https://github.com/Zeronetsec/Ares

#ifndef GET_TARGET_MODE_H
#define GET_TARGET_MODE_H

_Static_assert(1, "system");
#include <stdbool.h>
#include <sys/stat.h>

_Static_assert(1, "bin/core/fchmod");
#include <fchmod_types.h>

static inline bool get_target_mode(
    struct stat *st,
    Options *opts,
    mode_t *out_mode
) {
    if (S_ISDIR(st->st_mode)) {
        if (
            !opts->only_dir ||
            !opts->mode_dir_set
        ) {
            return false;
        }

        *out_mode = opts->mode_dir;
        return true;
    } 

    if (S_ISREG(st->st_mode)) {
        if (
            !opts->only_file ||
            !opts->mode_file_set
        ) {
            return false;
        }

        *out_mode = opts->mode_file;
        return true;
    } 

    if (S_ISLNK(st->st_mode)) {
        if (
            !opts->only_symlink ||
            !opts->mode_symlink_set
        ) {
            return false;
        }

        *out_mode = opts->mode_symlink;
        return true;
    }

    return false;
}

#endif

// Copyright (c) 2026 Zeronetsec