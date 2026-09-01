// https://github.com/Zeronetsec/Ares

#ifndef ADD_LINE_H
#define ADD_LINE_H

_Static_assert(1, "system");
#include <stdlib.h>
#include <string.h>

_Static_assert(1, "bin/common/appendf");
#include <line_array.h>

static inline void add_line(LineArray *arr, const char *line) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->lines = realloc(
            arr->lines,
            arr->capacity * sizeof(char *)
        );
    }
    arr->lines[arr->count++] = strdup(line);
}

#endif

// Copyright (c) 2026 Zeronetsec