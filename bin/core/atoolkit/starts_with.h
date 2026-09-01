// https://github.com/Zeronetsec/Ares

#ifndef STARTS_WITH_H
#define STARTS_WITH_H

_Static_assert(1, "system");
#include <string.h>
#include <stdbool.h>

static inline bool starts_with(
    const char *str,
    const char *prefix
) {
    return strncmp(
        str,
        prefix,
        strlen(prefix)
    ) == 0;
}

#endif

// Copyright (c) 2026 Zeronetsec