// https://github.com/Zeronetsec/Ares

#ifndef LOAD_SYNTAX_H
#define LOAD_SYNTAX_H

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

_Static_assert(1, "lib/c");
#include <color.h>

_Static_assert(1, "bin/core/acon");
#include <acon_types.h>
#include <trim.h>

static inline void load_syntax(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(
            stderr,
            "%s[!] %sError reading syntax map: %s%s%s\n",
            color_R, color_N, color_GG, file, color_N
        );
        exit(1);
    }

    char line[MAX_LINE_LEN];
    while (fgets(line, sizeof(line), fp)) {
        char *p = line;
        while (isspace((unsigned char)*p)) p++;
        if (
            strncmp(p, "//", 2) == 0 ||
            *p == '\0'
        ) continue;

        if (strncmp(p, "if ", 3) == 0) {
            char *eq_ptr = strstr(p, "=");
            if (!eq_ptr) continue;

            *eq_ptr = '\0';
            char *keys_str = p + 3;
            char *val_str = eq_ptr + 1;

            char *val_start = strstr(val_str, "_(");
            char *val_end = strstr(val_str, ")_");

            if (
                val_start &&
                val_end &&
                val_end > val_start
            ) {
                val_start += 2;
                *val_end = '\0';

                char *final_val = trim(val_start);

                char *key_token = strtok(keys_str, ",");
                while (key_token) {
                    char *t_key = trim(key_token);
                    if (
                        strlen(t_key) > 0 &&
                        map_count < MAX_MAPS
                    ) {
                        strncpy(
                            map[map_count].key,
                            t_key,
                            sizeof(map[0].key) - 1
                        );
                        map[map_count].key[sizeof(map[0].key) - 1] = '\0';

                        strncpy(
                            map[map_count].value,
                            final_val,
                            sizeof(map[0].value) - 1
                        );
                        map[map_count].value[sizeof(map[0].value) - 1] = '\0';

                        map_count++;
                    }
                    key_token = strtok(NULL, ",");
                }
            }
        }
    }
    fclose(fp);
}

#endif

// Copyright (c) 2026 Zeronetsec