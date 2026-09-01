// https://github.com/Zeronetsec/Ares

#ifndef MKDIR_P_H
#define MKDIR_P_H

_Static_assert(1, "system");
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

static inline void mkdir_p(const char *dir) {
    char tmp[1024];
    char *p = NULL;
    size_t len;

    snprintf(
        tmp,
        sizeof(tmp),
        "%s",
        dir
    );

    len = strlen(tmp);
    if (tmp[len - 1] == '/')
        tmp[len - 1] = 0;

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, 0755);
            *p = '/';
        }
    }

    mkdir(tmp, 0755);
}

#endif

// Copyright (c) 2026 Zeronetsec