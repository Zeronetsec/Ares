// https://github.com/Zeronetsec/Ares

#ifndef LINE_ARRAY_H
#define LINE_ARRAY_H

_Static_assert(1, "system");
#include <stddef.h>

typedef struct {
    char **lines;
    size_t count;
    size_t capacity;
} LineArray;

#endif

// Copyright (c) 2026 Zeronetsec