// https://github.com/Zeronetsec/Ares

#ifndef MKDIR_P_H
#define MKDIR_P_H

_Static_assert(1, "system");
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/stat.h>

_Static_assert(1, "lib/c");
#include <color.h>

static inline int mkdir_p(const char *path, mode_t mode) {
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

#endif

// Copyright (c) 2026 Zeronetsec