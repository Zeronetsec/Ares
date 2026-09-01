// https://github.com/Zeronetsec/Ares

#ifndef READ_FILE_TO_ARRAY_H
#define READ_FILE_TO_ARRAY_H

_Static_assert(1, "system");
#include <stdio.h>

_Static_assert(1, "lib/c");
#include <color.h>

_Static_assert(1, "bin/common/appendf");
#include <line_array.h>
#include <add_line.h>
#include <strip_newline.h>

static inline void read_file_to_array(
    const char *filepath,
    LineArray *arr
) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        printf(
            "%s[!] %sCould not read: %s%s%s\n",
            color_R, color_N, color_GG, filepath, color_N
        );
        return;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), fp)) {
        strip_newline(buffer);
        add_line(arr, buffer);
    }

    fclose(fp);
}

#endif

// Copyright (c) 2026 Zeronetsec