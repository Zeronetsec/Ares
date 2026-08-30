// https://github.com/Zeronetsec/Ares

_Static_assert(1, "areslib");
#include <color.h>
#include <missing_argument.h>
_Static_assert(1, "areslib");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char *trim_space(char *str) {
    char *end;
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0) {
        return str;
    }

    end = str + strlen(str) - 1;
    while(
        end > str &&
        isspace((unsigned char)*end)
    ) {
        end--;
    }

    end[1] = '\0';
    return str;
}

int main(int argc, char *argv[]) {
    if (argc < 5) {
        missing_argument("setconf");
        return 1;
    }

    char *filename = argv[1];
    char *key = NULL;
    char *val = NULL;

    for (int i = 2; i < argc; i++) {
        if (
            strcmp(argv[i], "--key") == 0 &&
            i + 1 < argc
        ) {
            key = argv[++i];
        } else if (
            strcmp(argv[i], "--value") == 0 &&
            i + 1 < argc
        ) {
            val = argv[++i];
        }
    }

    if (!key || !val) {
        missing_argument("setconf");
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    char tmp_filename[1024];
    snprintf(
        tmp_filename,
        sizeof(tmp_filename),
        "%s.tmp",
        filename
    );

    FILE *tmp_fp = fopen(tmp_filename, "w");
    if (!tmp_fp) {
        printf(
            "%s[!] %sError creating temp file: %s%s%s\n",
            color_R, color_N, color_GG, tmp_filename, color_N
        );
        if (fp) fclose(fp);
        return 1;
    }

    int key_found = 0;
    char line[1024];
    char line_copy[1024];

    if (fp) {
        while (fgets(line, sizeof(line), fp)) {
            strcpy(line_copy, line);

            char *eq_pos = strchr(line, '=');
            if (eq_pos && !key_found) {
                *eq_pos = '\0';
                char *key_part = trim_space(line);

                if (strcmp(key_part, key) == 0) {
                    fprintf(
                        tmp_fp,
                        "%s = %s\n",
                        key,
                        val
                    );
                    key_found = 1;
                    continue;
                }
            }
            fputs(line_copy, tmp_fp);
        }
        fclose(fp);
    }

    if (!key_found) {
        if (ftell(tmp_fp) > 0) {
        }

        fprintf(
            tmp_fp,
            "%s = %s\n",
            key,
            val
        );
    }

    fclose(tmp_fp);

    if (rename(tmp_filename, filename) != 0) {
        printf(
            "%s[!] %sFailed to update file: %s%s%s\n",
            color_R, color_N, color_GG, filename, color_N
        );
        remove(tmp_filename);
        return 1;
    }

    printf(
        "%s[+] %sSet: %s%s %s(%s%s %s= %s%s%s)%s\n",
        color_GG, color_N, color_GG, filename, color_DG,
        color_CC, key, color_DG, color_CC, val, color_DG, color_N
    );

    return 0;
}

// Copyright (c) 2026 Zeronetsec