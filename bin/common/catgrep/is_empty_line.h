// https://github.com/Zeronetsec/Ares

#ifndef IS_EMPTY_LINE_H
#define IS_EMPTY_LINE_H

_Static_assert(1, "system");
#include <stdbool.h>
#include <ctype.h>

static inline bool is_empty_line(const char *str) {
    while (*str) {
        if (!isspace((unsigned char)*str)) {
            return false;
        }
        str++;
    }
    return true;
}

#endif

// Copyright (c) 2026 Zeronetsec