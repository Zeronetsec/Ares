// https://github.com/Zeronetsec/Ares

#ifndef PROCESS_DIR_H
#define PROCESS_DIR_H

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>

_Static_assert(1, "lib/c");
#include <color.h>

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

static inline void process_dir(
    const char *dir_path,
    bool recursive,
    char **exts,
    int *ext_lens,
    int ext_count
) {
    DIR *dir = opendir(dir_path);
    if (!dir) return;

    char path_buf[PATH_MAX];
    char new_path_buf[PATH_MAX];
    
    size_t dir_len = strlen(dir_path);
    if (dir_len >= PATH_MAX - 2) { 
        closedir(dir); 
        return; 
    }

    memcpy(path_buf, dir_path, dir_len);
    if (path_buf[dir_len - 1] != '/') {
        path_buf[dir_len++] = '/';
    }
    path_buf[dir_len] = '\0';

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (
            entry->d_name[0] == '.' &&
            (
                entry->d_name[1] == '\0' || 
                (
                    entry->d_name[1] == '.' &&
                    entry->d_name[2] == '\0'
                )
            )
        ) {
            continue;
        }

        size_t name_len = strlen(entry->d_name);
        if (dir_len + name_len >= PATH_MAX) continue;

        memcpy(
            path_buf + dir_len,
            entry->d_name,
            name_len + 1
        );

        bool is_dir = false;
        bool is_reg = false;

        #ifdef _DIRENT_HAVE_D_TYPE
        if (entry->d_type != DT_UNKNOWN) {
            is_dir = (entry->d_type == DT_DIR);
            is_reg = (entry->d_type == DT_REG);
        } else
        #endif
        {
            struct stat st;
            if (lstat(path_buf, &st) == 0) {
                is_dir = S_ISDIR(st.st_mode);
                is_reg = S_ISREG(st.st_mode);
            }
        }

        if (is_dir && recursive) {
            process_dir(
                path_buf,
                recursive,
                exts,
                ext_lens,
                ext_count
            );
        } else if (is_reg) {
            int ext_match_len = 0;
            if (ext_count > 0) {
                for (int i = 0; i < ext_count; i++) {
                    if (name_len > ext_lens[i]) {
                        if (
                            memcmp(
                                entry->d_name + name_len - ext_lens[i],
                                exts[i],
                                ext_lens[i]
                            ) == 0
                        ) {
                            ext_match_len = ext_lens[i];
                            break;
                        }
                    }
                }
            } else {
                char *dot = strrchr(entry->d_name, '.');
                if (dot && dot != entry->d_name) {
                    ext_match_len = name_len - (
                        dot - entry->d_name
                    );
                }
            }

            if (ext_match_len > 0) {
                memcpy(
                    new_path_buf,
                    path_buf,
                    dir_len + name_len + 1
                );

                new_path_buf[
                    dir_len + name_len - ext_match_len
                ] = '\0';

                if (rename(path_buf, new_path_buf) == 0) {
                    printf(
                        "%s[*] %sRemoving extension: %s%s %s-> %s%s%s\n",
                        color_B, color_N, color_GG, path_buf, color_DG,
                        color_GG, new_path_buf, color_N
                    );
                } else {
                    printf(
                        "%s[!] %sRemoving extension failed: %s%s %s(%s%s%s)%s\n",
                        color_R, color_N, color_GG, path_buf, color_DG,
                        color_GG, strerror(errno), color_DG, color_N
                    );
                }
            }
        }
    }
    closedir(dir);
}

#endif

// Copyright (c) 2026 Zeronetsec