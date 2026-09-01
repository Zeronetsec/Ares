// https://github.com/Zeronetsec/Ares

#ifndef INIT_ARRAY_H
#define INIT_ARRAY_H

_Static_assert(1, "system");
#include <stdlib.h>

_Static_assert(1, "bin/common/appendf");
#include <line_array.h>

static inline void init_array(LineArray *arr) {
    arr->capacity = 1024;
    arr->count = 0;
    arr->lines = malloc(
        arr->capacity * sizeof(char *)
    );
}

#endif

// Copyright (c) 2026 Zeronetsec