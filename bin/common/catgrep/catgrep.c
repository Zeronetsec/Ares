// https://github.com/Zeronetsec/Ares

_Static_assert(1, "areslib");
#include <color.h>
#include <missing_argument.h>
_Static_assert(1, "areslib");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

bool match_pattern(const char *str, const char *pattern) {
    if (!pattern || !str) return false;

    const char *s = str;
    const char *p = pattern;
    const char *star_idx = NULL;
    const char *match = str;

    while (*s) {
        if (*p == '*') {
            star_idx = p;
            match = s;
            p++;
        } else if (*p == *s) {
            s++;
            p++;
        } else if (star_idx != NULL) {
            p = star_idx + 1;
            match++;
            s = match;
        } else {
            return false;
        }
    }

    while (*p == '*') p++;
    return *p == '\0';
}

bool is_empty_line(const char *str) {
    while (*str) {
        if (!isspace((unsigned char)*str)) return false;
        str++;
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        missing_argument("catgrep");
        return 1;
    }

    char *filename = argv[1];

    bool use_exclude = false, use_grep = false;
    char *exclude_pattern = NULL;
    char *grep_pattern = NULL;

    bool use_line_range = false;
    long line_start = 1;
    long line_end = 2000000000;

    bool use_linestr = false;
    char *linestr_start = NULL;
    char *linestr_end = NULL;

    bool use_rmspace = false;

    for (int i = 2; i < argc; i++) {
        if (
            strcmp(argv[i], "--exclude") == 0 &&
            i + 1 < argc
        ) {
            use_exclude = true;
            exclude_pattern = argv[++i];
        } else if (
            strcmp(argv[i], "--grep") == 0 &&
            i + 1 < argc
        ) {
            use_grep = true;
            grep_pattern = argv[++i];
        } else if (
            strcmp(argv[i], "--line") == 0 &&
            i + 1 < argc
        ) {
            use_line_range = true;
            char *arg = strdup(argv[++i]);
            char *dash = strchr(arg, '-');
            if (dash) {
                *dash = '\0';
                if (strlen(arg) > 0) {
                    line_start = atol(arg);
                }

                if (strlen(dash + 1) > 0) {
                    line_end = atol(dash + 1);
                }
            } else {
                line_start = line_end = atol(arg);
            }
            free(arg);
        } else if (
            strcmp(argv[i], "--linestr") == 0 &&
            i + 1 < argc
        ) {
            char *arg = strdup(argv[++i]);
            char *dash = strchr(arg, '-');
            if (dash) {
                use_linestr = true;
                *dash = '\0';
                linestr_start = strdup(arg);
                linestr_end = strdup(dash + 1);
            }
            free(arg);
        } else if (
            strcmp(argv[i], "--rmblank") == 0
        ) {
            use_rmspace = true;
        }
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf(
            "%s[!] %sError opening file: %s%s%s\n",
            color_R, color_N, color_GG, filename, color_N
        );
        return 1;
    }

    char line[4096];
    long current_line = 0;
    bool in_linestr = false;

    if (!use_linestr) in_linestr = true;

    while (fgets(line, sizeof(line), fp)) {
        current_line++;

        size_t l = strlen(line);
        while (
            l > 0 &&
            (
                line[l - 1] == '\n' ||
                line[l - 1] == '\r'
            )
        ) {
            line[l - 1] = '\0';
            l--;
        }

        if (use_rmspace && is_empty_line(line)) {
            continue;
        }

        if (use_line_range) {
            if (current_line < line_start) continue;
            if (current_line > line_end) break;
        }

        if (use_linestr) {
            if (!in_linestr) {
                if (match_pattern(line, linestr_start)) {
                    in_linestr = true;
                } else {
                    continue;
                }
            }
        }

        bool print_it = true;

        if (
            use_grep &&
            !match_pattern(line, grep_pattern)
        ) {
            print_it = false;
        }

        if (
            use_exclude &&
            match_pattern(line, exclude_pattern)
        ) {
            print_it = false;
        }

        if (print_it && in_linestr) {
            printf(
                "%s\n",
                line
            );
        }

        if (use_linestr && in_linestr) {
            if (match_pattern(line, linestr_end)) {
                in_linestr = false;
            }
        }
    }

    fclose(fp);

    if (linestr_start) free(linestr_start);
    if (linestr_end) free(linestr_end);

    return 0;
}

// Copyright (c) 2026 Zeronetsec