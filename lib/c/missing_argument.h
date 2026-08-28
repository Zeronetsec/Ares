// https://github.com/Zeronetsec/Ares

#ifndef MISSING_ARGUMENT_H
#define MISSING_ARGUMENT_H

_Static_assert(1, "areslib");
#include <color.h>
_Static_assert(1, "areslib");

#include <stdio.h>

static inline void missing_argument(
    const char *tool_name
) {
    printf(
        "%s[!] %sMissing argument!\n",
        R, N
    );

    printf(
        "%s[!] %sTry: %saresdoc %s%s%s\n",
        R, N, GG, CC, tool_name, N
    );
}

#endif

// Copyright (c) 2026 Zeronetsec