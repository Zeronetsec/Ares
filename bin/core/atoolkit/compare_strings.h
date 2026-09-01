// https://github.com/Zeronetsec/Ares

#ifndef COMPARE_STRINGS_H
#define COMPARE_STRINGS_H

_Static_assert(1, "system");
#include <string.h>

static inline int compare_strings(
    const void *a,
    const void *b
) {
    return strcmp(
        *(const char **)a,
        *(const char **)b
    );
}

#endif

// Copyright (c) 2026 Zeronetsec