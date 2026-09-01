// https://github.com/Zeronetsec/Ares

#ifndef PROCESS_TARGET_PATH_H
#define PROCESS_TARGET_PATH_H

_Static_assert(1, "system");
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>

_Static_assert(1, "lib/c");
#include <color.h>

_Static_assert(1, "bin/common/appendf");
#include <line_array.h>
#include <read_file_to_array.h>
#include <ends_with.h>

static inline void process_target_path(
    const char *path,
    bool recursive,
    const char *ext,
    LineArray *arr
) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        printf(
            "%s[!] %sPath: %s%s %snot found!\n",
            color_R, color_N, color_GG, path, color_N
        );
        return;
    }

    if (S_ISREG(st.st_mode)) {
        if (
            ext[0] == '\0' ||
            ends_with(path, ext)
        ) {
            read_file_to_array(path, arr);
        }
    } else if (S_ISDIR(st.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) return;
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

            struct stat child_st;
            if (lstat(next_path, &child_st) == 0) {
                if (
                    S_ISDIR(child_st.st_mode) &&
                    recursive
                ) {
                    process_target_path(
                        next_path,
                        recursive,
                        ext,
                        arr
                    );
                } else if (S_ISREG(child_st.st_mode)) {
                    if (
                        ext[0] == '\0' ||
                        ends_with(next_path, ext)
                    ) {
                        read_file_to_array(
                            next_path,
                            arr
                        );
                    }
                }
            }
        }
        closedir(dir);
    }
}

#endif

// Copyright (c) 2026 Zeronetsec