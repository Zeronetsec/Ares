// https://github.com/Zeronetsec/Ares

#ifndef INVALID_OPTION_H
#define INVALID_OPTION_H

_Static_assert(1, "system");
#include <stdio.h>

_Static_assert(1, "lib/c");
#include <color.h>

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