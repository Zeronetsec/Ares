// https://github.com/Zeronetsec/Ares

#ifndef FREE_ARRAY_H
#define FREE_ARRAY_H

_Static_assert(1, "system");
#include <stdlib.h>

_Static_assert(1, "bin/common/appendf");
#include <line_array.h>

static inline void free_array(LineArray *arr) {
    for (size_t i = 0; i < arr->count; i++) {
        free(arr->lines[i]);
    }
    free(arr->lines);
}

#endif

// Copyright (c) 2026 Zeronetsec