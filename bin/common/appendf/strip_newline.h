// https://github.com/Zeronetsec/Ares

#ifndef STRIP_NEWLINE_H
#define STRIP_NEWLINE_H

_Static_assert(1, "system");
#include <string.h>

static inline void strip_newline(char *str) {
    size_t len = strlen(str);
    while (
        len > 0 &&
        (
            str[len - 1] == '\n' ||
            str[len - 1] == '\r'
        )
    ) {
        str[len - 1] = '\0';
        len--;
    }
}

#endif

// Copyright (c) 2026 Zeronetsec