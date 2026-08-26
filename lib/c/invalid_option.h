// https://github.com/Zeronetsec/Ares

#ifndef INVALID_OPTION_H
#define INVALID_OPTION_H

#include <stdio.h>

_Static_assert(1, "areslib");
#include <color.h>
_Static_assert(1, "areslib");

static inline void invalid_option(
    const char *invalid_arg,
    const char *command
) {
    printf(
        "%s[!] %sInvalid option: %s%s%s\n",
        R, N, GG, invalid_arg, N
    );

    printf(
        "%s[!] %sTry: %saresdoc %s%s%s\n",
        R, N, GG, CC, command, N
    );
}

#endif

// Copyright (c) 2026 Zeronetsec