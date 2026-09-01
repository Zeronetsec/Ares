// https://github.com/Zeronetsec/Ares

#ifndef PRINT_UNESCAPED_H
#define PRINT_UNESCAPED_H

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

static inline void print_unescaped(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (
            str[i] == '\\' &&
            str[i+1] != '\0'
        ) {
            if (
                str[i+1] == 'x' &&
                isxdigit((unsigned char)str[i+2]) &&
                isxdigit((unsigned char)str[i+3])
            ) {
                char hex[3] = {str[i+2], str[i+3], '\0'};
                putchar(
                    (char)strtol(
                        hex,
                        NULL,
                        16
                    )
                );
                i += 3;
            } else if (str[i+1] == 'e') {
                putchar('\x1b');
                i++;
            } else if (str[i+1] == 'n') {
                putchar('\n');
                i++;
            } else if (str[i+1] == 't') {
                putchar('\t');
                i++;
            } else if (str[i+1] == 'r') {
                putchar('\r');
                i++;
            } else if (str[i+1] == '\\') {
                putchar('\\');
                i++;
            } else {
                putchar(str[i]);
            }
        } else {
            putchar(str[i]);
        }
    }
}

#endif

// Copyright (c) 2026 Zeronetsec