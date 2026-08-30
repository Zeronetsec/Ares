// https://github.com/Zeronetsec/Ares

#ifndef INVALID_OPTION_H
#define INVALID_OPTION_H

_Static_assert(1, "areslib");
#include <color.h>
_Static_assert(1, "areslib");

#include <stdio.h>

static inline void invalid_option(
    const char *invalid_arg,
    const char *command
) {
    printf(
        "%s[!] %sInvalid option: %s%s%s\n",
        color_R, color_N, color_GG, invalid_arg, color_N
    );

    printf(
        "%s[!] %sTry: %saresdoc %s%s%s\n",
        color_R, color_N, color_GG, color_CC, command, color_N
    );
}

#endif

// Copyright (c) 2026 Zeronetsec