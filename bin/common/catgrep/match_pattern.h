// https://github.com/Zeronetsec/Ares

#ifndef MATCH_PATTERN_H
#define MATCH_PATTERN_H

_Static_assert(1, "system");
#include <stdbool.h>
#include <stddef.h>

static inline bool match_pattern(
    const char *str,
    const char *pattern
) {
    if (!pattern || !str) return false;

    const char *s = str;
    const char *p = pattern;
    const char *star_idx = NULL;
    const char *match = str;

    while (*s) {
        if (*p == '*') {
            star_idx = p;
            match = s;
            p++;
        } else if (*p == *s) {
            s++;
            p++;
        } else if (star_idx != NULL) {
            p = star_idx + 1;
            match++;
            s = match;
        } else {
            return false;
        }
    }

    while (*p == '*') p++;
    return *p == '\0';
}

#endif

// Copyright (c) 2026 Zeronetsec