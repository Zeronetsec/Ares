// https://github.com/Zeronetsec/Ares

#ifndef REMOVE_RECURSIVE_H
#define REMOVE_RECURSIVE_H

#define _XOPEN_SOURCE 500

_Static_assert(1, "system");
#include <ftw.h>

_Static_assert(1, "bin/core/remake");
#include <unlink_cb.h>

static inline int remove_recursive(const char *path) {
    return nftw(
        path,
        unlink_cb,
        256,
        FTW_DEPTH | FTW_PHYS
    );
}

#endif

// Copyright (c) 2026 Zeronetsec