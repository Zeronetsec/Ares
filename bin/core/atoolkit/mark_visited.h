// https://github.com/Zeronetsec/Ares

#ifndef MARK_VISITED_H
#define MARK_VISITED_H

_Static_assert(1, "system");
#include <stdlib.h>
#include <sys/types.h>

_Static_assert(1, "bin/core/atoolkit");
#include <atoolkit_types.h>

static inline void mark_visited(dev_t dev, ino_t ino) {
    if (visited_count >= visited_capacity) {
        visited_capacity = visited_capacity == 0 ?
            16 :
            visited_capacity * 2;

        visited_dirs = realloc(
            visited_dirs,
            visited_capacity * sizeof(VisitedDir)
        );
    }

    visited_dirs[visited_count].dev = dev;
    visited_dirs[visited_count].ino = ino;

    visited_count++;
}

#endif

// Copyright (c) 2026 Zeronetsec