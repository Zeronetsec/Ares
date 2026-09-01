// https://github.com/Zeronetsec/Ares

#ifndef MISSING_ARGUMENT_H
#define MISSING_ARGUMENT_H

_Static_assert(1, "system");
#include <stdio.h>

_Static_assert(1, "lib/c");
#include <color.h>

static inline void missing_argument(
    const char *tool_name
) {
    printf(
        "%s[!] %sMissing argument!\n",
        color_R, color_N
    );

    printf(
        "%s[!] %sTry: %saresdoc %s%s%s\n",
        color_R, color_N, color_GG, color_CC, tool_name, color_N
    );
}

#endif

// Copyright (c) 2026 Zeronetsec