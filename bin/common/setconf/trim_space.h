// https://github.com/Zeronetsec/Ares

#ifndef TRIM_SPACE_H
#define TRIM_SPACE_H

_Static_assert(1, "system");
#include <ctype.h>
#include <string.h>

static inline char *trim_space(char *str) {
    char *end;
    while (isspace((unsigned char)*str)) str++;

    if (*str == 0) {
        return str;
    }

    end = str + strlen(str) - 1;
    while (
        end > str &&
        isspace((unsigned char)*end)
    ) {
        end--;
    }

    end[1] = '\0';
    return str;
}

#endif

// Copyright (c) 2026 Zeronetsec