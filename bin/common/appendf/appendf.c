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
#include <dirent.h>
#include <sys/stat.h>

typedef struct {
    char **lines;
    size_t count;
    size_t capacity;
} LineArray;

void init_array(LineArray *arr) {
    arr->capacity = 1024;
    arr->count = 0;
    arr->lines = malloc(
        arr->capacity * sizeof(char *)
    );
}

void add_line(LineArray *arr, const char *line) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->lines = realloc(
            arr->lines,
            arr->capacity * sizeof(char *)
        );
    }
    arr->lines[arr->count++] = strdup(line);
}

void free_array(LineArray *arr) {
    for (size_t i = 0; i < arr->count; i++) {
        free(arr->lines[i]);
    }
    free(arr->lines);
}

void strip_newline(char *str) {
    size_t len = strlen(str);
    while (
        len > 0 &&
        (
            str[len - 1] == '\n' ||
            str[len - 1] == '\r'
        )
    ) {
        str[len - 1] = '\0';
        len--;
    }
}

void read_file_to_array(const char *filepath, LineArray *arr) {
    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        printf(
            "%s[!] %sCould not read: %s%s%s\n",
            R, N, GG, filepath, N
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

bool ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) {
        return false;
    }

    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);

    if (len_suffix > len_str) {
        return false;
    }

    return strncmp(
        str + len_str - len_suffix,
        suffix,
        len_suffix
    ) == 0;
}

void process_target_path(
    const char *path,
    bool recursive,
    const char *ext,
    LineArray *arr
) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        printf(
            "%s[!] %sPath: %s%s %snot found!\n",
            R, N, GG, path, N
        );
        return;
    }

    if (S_ISREG(st.st_mode)) {
        if (
            ext[0] == '\0' ||
            ends_with(path, ext)
        ) {
            read_file_to_array(path, arr);
        }
    } else if (S_ISDIR(st.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) return;
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            if (
                strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0
            ) {
                continue;
            }

            char next_path[4096];
            snprintf(
                next_path,
                sizeof(next_path),
                "%s/%s",
                path,
                entry->d_name
            );

            struct stat child_st;
            if (lstat(next_path, &child_st) == 0) {
                if (
                    S_ISDIR(child_st.st_mode) &&
                    recursive
                ) {
                    process_target_path(
                        next_path,
                        recursive,
                        ext,
                        arr
                    );
                } else if (S_ISREG(child_st.st_mode)) {
                    if (
                        ext[0] == '\0' ||
                        ends_with(next_path, ext)
                    ) {
                        read_file_to_array(
                            next_path,
                            arr
                        );
                    }
                }
            }
        }
        closedir(dir);
    }
}

int cmp_str(const void *a, const void *b) {
    const char *str_a = *(const char **)a;
    const char *str_b = *(const char **)b;
    return strcmp(str_a, str_b);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        missing_argument("appendf");
        return 1;
    }

    LineArray all_lines;
    init_array(&all_lines);

    LineArray exc_lines;
    init_array(&exc_lines);

    char *target_arg = NULL;
    char *out_file = NULL;

    bool do_sort = false;
    bool do_rmdup = false;

    for (int i = 1; i < argc; i++) {
        if (
            strcmp(argv[i], "--target") == 0 &&
            i + 1 < argc
        ) {
            target_arg = argv[++i];
        } else if (
            strcmp(argv[i], "--out") == 0 &&
            i + 1 < argc
        ) {
            out_file = argv[++i];
        } else if (
            strcmp(argv[i], "--sort") == 0
        ) {
            do_sort = true;
        } else if (
            strcmp(argv[i], "--rmdup") == 0
        ) {
            do_rmdup = true;
        } else if (
            strcmp(argv[i], "--include") == 0 &&
            i + 1 < argc
        ) {
            add_line(
                &all_lines,
                argv[++i]
            );
        } else if (
            strcmp(argv[i], "--includef") == 0 &&
            i + 1 < argc
        ) {
            read_file_to_array(
                argv[++i],
                &all_lines
            );
        } else if (
            strcmp(argv[i], "--exclude") == 0 &&
            i + 1 < argc
        ) {
            add_line(
                &exc_lines,
                argv[++i]
            );
        } else if (
            strcmp(argv[i], "--excludef") == 0 &&
            i + 1 < argc
        ) {
            read_file_to_array(
                argv[++i],
                &exc_lines
            );
        }
    }

    if (!out_file) {
        missing_argument("appendf");
        return 1;
    }

    if (target_arg) {
        char *target_copy = strdup(target_arg);

        char *start = target_copy;
        if (*start == '[') start++;

        size_t len = strlen(start);
        if (
            len > 0 &&
            start[len - 1] == ']'
        ) {
            start[len - 1] = '\0';
        }

        char *token = strtok(start, ",");
        while (token != NULL) {
            while (isspace((unsigned char)*token)) {
                token++;
            }

            char base_path[1024];
            bool is_recursive = false;
            char ext_str[256] = "";

            char *rec_ptr = strstr(
                token,
                ".recursive("
            );

            char *ext_ptr = strstr(
                token,
                ".ext("
            );

            char *cut_pos = token + strlen(token);
            if (
                rec_ptr &&
                rec_ptr < cut_pos
            ) {
                cut_pos = rec_ptr;
            }

            if (
                ext_ptr &&
                ext_ptr < cut_pos
            ) {
                cut_pos = ext_ptr;
            }

            strncpy(
                base_path,
                token,
                cut_pos - token
            );
            base_path[cut_pos - token] = '\0';

            if (rec_ptr) {
                if (
                    strncmp(
                        rec_ptr + 11,
                        "true",
                        4
                    ) == 0
                ) {
                    is_recursive = true;
                }
            }

            if (ext_ptr) {
                char *end_paren = strchr(ext_ptr + 5, ')');
                if (end_paren) {
                    size_t elen = end_paren - (ext_ptr + 5);
                    strncpy(
                        ext_str,
                        ext_ptr + 5,
                        elen
                    );
                    ext_str[elen] = '\0';
                }
            }

            process_target_path(
                base_path,
                is_recursive,
                ext_str,
                &all_lines
            );
            token = strtok(NULL, ",");
        }
        free(target_copy);
    }

    if (exc_lines.count > 0) {
        qsort(
            exc_lines.lines,
            exc_lines.count,
            sizeof(char *),
            cmp_str
        );
    }

    LineArray filtered_lines;
    init_array(&filtered_lines);

    for (size_t i = 0; i < all_lines.count; i++) {
        bool drop = false;
        if (exc_lines.count > 0) {
            if (
                bsearch(
                    &all_lines.lines[i],
                    exc_lines.lines,
                    exc_lines.count,
                    sizeof(char *),
                    cmp_str
                )
            ) {
                drop = true;
            }
        }
        if (!drop) {
            add_line(
                &filtered_lines,
                all_lines.lines[i]
            );
        }
    }

    if (do_sort && filtered_lines.count > 0) {
        qsort(
            filtered_lines.lines,
            filtered_lines.count,
            sizeof(char *),
            cmp_str
        );
    }

    FILE *out_fp = fopen(out_file, "w");
    if (!out_fp) {
        printf(
            "%s[!] %sCould not open output file: %s%s%s\n",
            R, N, GG, out_file, N
        );
        return 1;
    }

    size_t written = 0;
    for (size_t i = 0; i < filtered_lines.count; i++) {
        if (do_rmdup) {
            if (do_sort) {
                if (
                    i > 0 &&
                    strcmp(
                        filtered_lines.lines[i],
                        filtered_lines.lines[i - 1]
                    ) == 0
                ) {
                    continue;
                }
            } else {
                bool is_dup = false;
                for (size_t j = 0; j < i; j++) {
                    if (
                        strcmp(
                            filtered_lines.lines[i],
                            filtered_lines.lines[j]
                        ) == 0
                    ) {
                        is_dup = true; break;
                    }
                }
                if (is_dup) continue;
            }
        }
        fprintf(
            out_fp,
            "%s\n",
            filtered_lines.lines[i]
        );
        written++;
    }
    fclose(out_fp);

    printf(
        "%s[+] %sSuccessfully compiled: %s%zu %slines to %s%s%s\n", 
        GG, N, GG, written, N, GG, out_file, N
    );

    free_array(&all_lines);
    free_array(&exc_lines);
    free_array(&filtered_lines);

    return 0;
}

// Copyright (c) 2026 Zeronetsec