// https://github.com/Zeronetsec/Ares

#ifndef TRIM_H
#define TRIM_H

_Static_assert(1, "system");
#include <string.h>
#include <ctype.h>

static inline char* trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    char *end = s + strlen(s) - 1;

    while (
        end > s &&
        isspace((unsigned char)*end)
    ) end--;

    end[1] = '\0';
    return s;
}

#endif

// Copyright (c) 2026 Zeronetsec