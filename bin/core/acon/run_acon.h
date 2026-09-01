// https://github.com/Zeronetsec/Ares

#ifndef RUN_ACON_H
#define RUN_ACON_H

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

_Static_assert(1, "lib/c");
#include <color.h>

_Static_assert(1, "bin/core/acon");
#include <acon_types.h>
#include <find_map.h>
#include <print_unescaped.h>

static inline void run_acon(const char *file, int raw_mode) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(
            stderr,
            "%s[!] %sError reading file: %s%s%s\n",
            color_R, color_N, color_GG, file, color_N
        );
        exit(1);
    }

    if (raw_mode == 1) {
        int c;
        while ((c = fgetc(fp)) != EOF) putchar(c);
        fclose(fp);
        return;
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        while (
            isspace((unsigned char)*p) &&
            *p != '\n' &&
            *p != '\r'
        ) p++;

        if (strncmp(p, "//", 2) == 0) {
            continue;
        }

        for (int i = 0; line[i] != '\0'; i++) {
            if (line[i] == '{') {
                int j = i + 1;
                while (
                    line[j] != '\0' &&
                    line[j] != '}'
                ) j++;

                if (line[j] == '}') {
                    char key[128];
                    int len = j - i - 1;
                    if (len < (int)sizeof(key)) {
                        strncpy(key, line + i + 1, len);
                        key[len] = '\0';

                        char *val = find_map(key);
                        if (val) {
                            if (raw_mode == 2) {
                                fputs(val, stdout);
                            } else {
                                print_unescaped(val);
                            }
                        } else {
                            printf("{%s}", key);
                        }
                        i = j;
                        continue;
                    }
                }
            }
            putchar(line[i]);
        }
    }
    fclose(fp);
}

#endif

// Copyright (c) 2026 Zeronetsec