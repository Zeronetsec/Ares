// https://github.com/Zeronetsec/Ares

// Usage: getconf <file> --key <key> [--auto <lower|upper>] [--get <index>]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <color.h>
#include <missing_argument.h>

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
    if (argc < 4) {
        missing_argument("getconf");
        return 1;
    }

    char *filename = argv[1];
    char *key = NULL;
    int auto_case = 0;
    int get_idx = 0;

    for (int i = 2; i < argc; i++) {
        if (
            strcmp(argv[i], "--key") == 0 &&
            i + 1 < argc
        ) {
            key = argv[++i];
        } else if (
            strcmp(argv[i], "--auto") == 0 &&
            i + 1 < argc
        ) {
            i++;
            if (
                strcmp(argv[i], "lower") == 0
            ) {
                auto_case = 1;
            } else if (
                strcmp(argv[i], "upper") == 0
            ) {
                auto_case = 2;
            }
        } else if (
            strcmp(argv[i], "--get") == 0 &&
            i + 1 < argc
        ) {
            get_idx = atoi(argv[++i]);
        }
    }

    if (!key) {
        missing_argument("getconf");
        return 1;
    }

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf(
            "%s[!] %sError to opening: %s%s%s\n",
            R, N, GG, filename, N
        );
        return 1;
    }

    char line[1024];
    char *found_val = NULL;

    while (fgets(line, sizeof(line), fp)) {
        char *eq_pos = strchr(line, '=');
        if (eq_pos) {
            *eq_pos = '\0';

            char *key_part = trim_space(line);
            char *val_part = trim_space(eq_pos + 1);

            if (strcmp(key_part, key) == 0) {
                found_val = val_part;
                break;
            }
        }
    }

    fclose(fp);

    if (!found_val) {
        printf(
            "%s[!] %sKey: %s%s %snot found!\n",
            R, N, GG, key, N
        );
        return 1;
    }

    char *final_val = found_val;
    if (get_idx > 0) {
        char *token = strtok(
            found_val,
            " \t"
        );

        int count = 1;
        int token_found = 0;

        while (token != NULL) {
            if (count == get_idx) {
                final_val = token;
                token_found = 1;
                break;
            }

            token = strtok(
                NULL,
                " \t"
            );
            count++;
        }

        if (!token_found) return 1;
    }

    for (int i = 0; final_val[i]; i++) {
        if (auto_case == 1) {
            final_val[i] = tolower((unsigned char)final_val[i]);
        } else if (auto_case == 2) {
            final_val[i] = toupper((unsigned char)final_val[i]);
        }
    }

    printf(
        "%s\n",
        final_val
    );

    return 0;
}

// Copyright (c) 2026 Zeronetsec