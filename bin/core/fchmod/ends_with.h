// https://github.com/Zeronetsec/Ares

#ifndef ENDS_WITH_H
#define ENDS_WITH_H

_Static_assert(1, "system");
#include <string.h>
#include <stdbool.h>

static inline bool ends_with(
    const char *str,
    const char *suffix
) {
    if (!str || !suffix) return false;

    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);
    if (len_suffix > len_str) return false;

    return strncmp(
        str + len_str - len_suffix,
        suffix, len_suffix
    ) == 0;
}

#endif

// Copyright (c) 2026 Zeronetsec