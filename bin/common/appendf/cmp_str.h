// https://github.com/Zeronetsec/Ares

#ifndef CMP_STR_H
#define CMP_STR_H

_Static_assert(1, "system");
#include <string.h>

static inline int cmp_str(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strcmp(str_a, str_b);
}

#endif

// Copyright (c) 2026 Zeronetsec