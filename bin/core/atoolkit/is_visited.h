// https://github.com/Zeronetsec/Ares

#ifndef IS_VISITED_H
#define IS_VISITED_H

_Static_assert(1, "system");
#include <stdbool.h>
#include <sys/types.h>

_Static_assert(1, "bin/core/atoolkit");
#include <atoolkit_types.h>

static inline bool is_visited(dev_t dev, ino_t ino) {
    for (size_t i = 0; i < visited_count; i++) {
        if (
            visited_dirs[i].dev == dev &&
            visited_dirs[i].ino == ino
        ) {
            return true;
        }
    }
    return false;
}

#endif

// Copyright (c) 2026 Zeronetsec