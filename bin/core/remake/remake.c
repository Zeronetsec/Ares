// https://github.com/Zeronetsec/Ares

_Static_assert(1, "areslib");
#include <color.h>
#include <missing_argument.h>
#include <invalid_option.h>
_Static_assert(1, "areslib");

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ftw.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

int unlink_cb(
    const char *fpath,
    const struct stat *sb,
    int typeflag,
    struct FTW *ftwbuf
) {
    int rv;
    if (
        typeflag == FTW_DP ||
        typeflag == FTW_D
    ) {
        rv = rmdir(fpath);
        if (rv == 0) {
            printf(
                "%s[-] %sRemoved directory: %s%s%s\n",
                color_YY, color_N, color_GG, fpath, color_N
            );
        }
    } else {
        rv = unlink(fpath);
        if (rv == 0) {
            printf(
                "%s[-] %sRemoved file: %s%s%s\n",
                color_YY, color_N, color_GG, fpath, color_N
            );
        }
    }
    
    if (rv != 0) perror(fpath);
    return rv;
}

int remove_recursive(const char *path) {
    return nftw(
        path,
        unlink_cb,
        256,
        FTW_DEPTH | FTW_PHYS
    );
}

int mkdir_p(const char *path, mode_t mode) {
    char tmp[4096];
    snprintf(
        tmp,
        sizeof(tmp),
        "%s",
        path
    );
    size_t len = strlen(tmp);

    if (
        len > 0 &&
        tmp[len-1] == '/'
    ) {
        tmp[len-1] = '\0';
    }

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0777) == 0) {
                printf(
                    "%s[+] %sCreated directory: %s%s%s\n",
                    color_GG, color_N, color_GG, tmp, color_N
                );
            }
            *p = '/';
        }
    }
    
    if (mkdir(tmp, mode) == 0) {
        printf(
            "%s[+] %sCreated directory: %s%s%s\n",
            color_GG, color_N, color_GG, tmp, color_N
        );
        return 0;
    } else if (errno == EEXIST) {
        return 0;
    }
    return -1;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        missing_argument("remake");
        return 1;
    }

    char *target = NULL;
    mode_t mode = 0;
    bool mode_set = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--mode") == 0) {
            if (i + 1 < argc) {
                mode = strtol(argv[++i], NULL, 8);
                mode_set = true;
            } else {
                char invalid_args[256]; 
                snprintf(
                    invalid_args,
                    sizeof(invalid_args),
                    "--mode %s",
                    argv[i]
                );

                invalid_option(
                    invalid_args,
                    "remake"
                );
                return 1;
            }
        } else {
            target = argv[i];
        }
    }

    if (!target) {
        missing_argument("remake");
        return 1;
    }

    bool is_dir = false;
    struct stat st;
    if (stat(target, &st) == 0) {
        is_dir = S_ISDIR(st.st_mode);
    } else {
        size_t len = strlen(target);
        if (len > 0 && target[len - 1] == '/') {
            is_dir = true;
        }
    }

    if (access(target, F_OK) == 0) {
        if (is_dir) {
            remove_recursive(target);
        } else {
            if (unlink(target) == 0) {
                printf(
                    "%s[!] %sRemoved: %s%s%s\n",
                    color_R, color_N, color_GG, target, color_N
                );
            } else {
                perror("unlink");
            }
        }
    }

    mode_t dir_mode = mode_set ?
        mode :
        0777;

    mode_t file_mode = mode_set ?
        mode :
        0666;

    if (is_dir) {
        mkdir_p(target, dir_mode);
        if (mode_set) chmod(target, mode); 
    } else {
        char tmp_path[4096];
        snprintf(
            tmp_path,
            sizeof(tmp_path),
            "%s",
            target
        );
        char *last_slash = strrchr(tmp_path, '/');
        
        if (last_slash != NULL) {
            *last_slash = '\0';
            mkdir_p(tmp_path, 0777);
        }

        int fd = open(
            target, O_CREAT | O_WRONLY | O_TRUNC,
            file_mode
        );

        if (fd >= 0) {
            close(fd);
            printf(
                "%s[+] %sCreated file: %s%s%s\n",
                color_GG, color_N, color_GG, target, color_N
            );
            if (mode_set) {
                chmod(target, mode);
            }
        } else {
            perror("open");
        }
    }

    return 0;
}

// Copyright (c) 2026 Zeronetsec