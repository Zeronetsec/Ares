// https://github.com/Zeronetsec/Ares

_Static_assert(1, "areslib");
#include <color.h>
#include <missing_argument.h>
_Static_assert(1, "areslib");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define MAX_MAPS 1024
#define MAX_LINE_LEN 4096

typedef struct {
    char key[128];
    char value[512];
} MapEntry;

MapEntry map[MAX_MAPS];
int map_count = 0;

char* trim(char *s) {
    while (isspace((unsigned char)*s)) s++;
    if (*s == 0) return s;
    char *end = s + strlen(s) - 1;

    while (
        end > s &&
        isspace((unsigned char)*end)
    ) end--;

    end[1] = '\0';
    return s;
}

void print_unescaped(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '\\' && str[i+1] != '\0') {
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

void load_syntax(const char *file) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(
            stderr,
            "%s[!] %sError reading syntax map: %s%s%s\n",
            R, N, GG, file, N
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

char* find_map(const char *key) {
    for (int i = 0; i < map_count; i++) {
        if (strcmp(map[i].key, key) == 0) {
            return map[i].value;
        }
    }
    return NULL;
}

void run_acon(const char *file, int raw_mode) {
    FILE *fp = fopen(file, "r");
    if (!fp) {
        fprintf(
            stderr,
            "%s[!] %sError reading file: %s%s%s\n",
            R, N, GG, file, N
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

int main(int argc, char *argv[]) {
    if (argc < 2) {
        missing_argument("acon");
        return 0;
    }

    char *acon_file = NULL;
    char *syntax_file = NULL;
    int raw_mode = 0;

    for (int i = 1; i < argc; i++) {
        if (
            strcmp(argv[i], "--map") == 0
            && i + 1 < argc
        ) {
            syntax_file = argv[++i];
        } else if (
            strcmp(argv[i], "--raw") == 0
            && i + 1 < argc
        ) {
            i++;
            if (
                strcmp(argv[i], "full") == 0
            ) {
                raw_mode = 1;
            } else if (
                strcmp(argv[i], "escape") == 0
            ) {
                raw_mode = 2;
            }
        } else if (
            argv[i][0] != '-'
        ) {
            acon_file = argv[i];
        }
    }

    if (!acon_file) {
        missing_argument("acon");
        return 1;
    }

    char default_map[1024];
    if (!syntax_file) {
        char *config_dir = getenv("__config__");
        if (config_dir) {
            snprintf(
                default_map,
                sizeof(default_map),
                "%s/acon/acon.syntax",
                config_dir
            );
            syntax_file = default_map;
        } else {
            fprintf(
                stderr,
                "%s[!] %sDefault syntax map is empty!\n",
                R, N
            );
            return 1;
        }
    }

    if (raw_mode != 1) {
        load_syntax(syntax_file);
    }

    run_acon(acon_file, raw_mode);
    return 0;
}

// Copyright (c) 2026 Zeronetsec