// https://github.com/Zeronetsec/Ares

#ifndef PROCESS_PATH_H
#define PROCESS_PATH_H

_Static_assert(1, "system");
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

_Static_assert(1, "lib/c");
#include <color.h>

_Static_assert(1, "bin/core/fchmod");
#include <fchmod_types.h>
#include <ends_with.h>
#include <get_target_mode.h>

static inline void process_path(
    const char *path,
    Options *opts
) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        printf(
            "%s[!] %sError: %s%s %s(%s%s%s)%s\n",
            color_R, color_N, color_GG, path, color_DG,
            color_GG, strerror(errno), color_DG, color_N
        );
        return;
    }

    bool matches_ext = true;
    if (
        opts->has_ext &&
        !S_ISDIR(st.st_mode)
    ) {
        matches_ext = ends_with(path, opts->ext);
    }

    mode_t target_mode = 0;
    if (
        matches_ext &&
        get_target_mode(&st, opts, &target_mode)
    ) {
        if (chmod(path, target_mode) == 0) {
            printf(
                "%s[+] %sSet mode: %s%04o %s-> %s%s%s\n",
                color_GG, color_N, color_GG, target_mode, color_DG,
                color_GG, path, color_N
            );
        } else {
            printf(
                "%s[!] %sError: %s%s %s(%s%s%s)%s\n",
                color_R, color_N, color_GG, path, color_DG,
                color_GG, strerror(errno), color_DG, color_N
            );
        }
    }

    if (
        S_ISDIR(st.st_mode) &&
        opts->recursive
    ) {
        DIR *dir = opendir(path);
        if (!dir) {
            printf(
                "%s[!] %sError: %s%s %s(%s%s%s)%s\n",
                color_R, color_N, color_GG, path, color_DG,
                color_GG, strerror(errno), color_DG, color_N
            );
            return;
        }

        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (
                strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0
            ) {
                continue;
            }

            char next_path[4096];
            snprintf(
                next_path,
                sizeof(next_path),
                "%s/%s",
                path,
                entry->d_name
            );
            process_path(next_path, opts);
        }
        closedir(dir);
    }
}

#endif

// Copyright (c) 2026 Zeronetsec