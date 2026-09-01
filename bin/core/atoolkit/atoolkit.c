// https://github.com/Zeronetsec/Ares

_Static_assert(1, "areslib");
#include <color.h>
#include <missing_argument.h>
#include <invalid_option.h>
_Static_assert(1, "areslib");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

typedef struct {
    dev_t dev;
    ino_t ino;
} VisitedDir;

VisitedDir *visited_dirs = NULL;
size_t visited_count = 0;
size_t visited_capacity = 0;

bool is_visited(dev_t dev, ino_t ino) {
    for (size_t i = 0; i < visited_count; i++) {
        if (
            visited_dirs[i].dev == dev &&
            visited_dirs[i].ino == ino
        ) {
            return true;
        }
    }
    return false;
}

void mark_visited(dev_t dev, ino_t ino) {
    if (visited_count >= visited_capacity) {
        visited_capacity = visited_capacity == 0 ?
            16 :
            visited_capacity * 2;

        visited_dirs = realloc(
            visited_dirs,
            visited_capacity * sizeof(VisitedDir)
        );
    }

    visited_dirs[visited_count].dev = dev;
    visited_dirs[visited_count].ino = ino;

    visited_count++;
}

bool starts_with(const char *str, const char *prefix) {
    return strncmp(
        str,
        prefix,
        strlen(prefix)
    ) == 0;
}

void rstrip_slash(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    while (len > 0 && str[len - 1] == '/') {
        str[len - 1] = '\0';
        len--;
    }
}

char *strip_slashes(char *str) {
    if (!str) return NULL;
    while (*str == '/') str++;
    rstrip_slash(str);
    return str;
}

int compare_strings(const void *a, const void *b) {
    return strcmp(
        *(const char **)a,
        *(const char **)b
    );
}

void get_clean_path(
    const char *path,
    char *out_buf,
    size_t out_sz
) {
    char norm_path[PATH_MAX];
    if (!realpath(path, norm_path)) {
        snprintf(
            out_buf,
            out_sz,
            "%s",
            path
        );
        return;
    }

    const char *env_ares = getenv("__aresroot__");
    char ares_root[PATH_MAX] = {0};
    if (env_ares) {
        strncpy(
            ares_root,
            env_ares,
            PATH_MAX - 1
        );
        rstrip_slash(ares_root);
    }

    const char *env_pref = getenv("PREFIX");
    char sys_prefix[PATH_MAX] = {0};
    if (env_pref) {
        strncpy(
            sys_prefix,
            env_pref,
            PATH_MAX - 1
        );
        rstrip_slash(sys_prefix);
    }

    if (
        strlen(ares_root) > 0 &&
        starts_with(norm_path, ares_root)
    ) {
        const char *rel = norm_path + strlen(ares_root);
        if (*rel == '\0') {
            snprintf(
                out_buf,
                out_sz,
                "ares"
            );
        } else if (*rel == '/') {
            snprintf(
                out_buf,
                out_sz,
                "ares%s",
                rel
            );
        }
        return;
    }

    if (
        strlen(sys_prefix) > 0 &&
        starts_with(norm_path, sys_prefix)
    ) {
        char *last_slash = strrchr(sys_prefix, '/');
        if (last_slash) {
            *last_slash = '\0';
            if (starts_with(norm_path, sys_prefix)) {
                const char *rel = norm_path + strlen(sys_prefix);
                if (*rel == '/') rel++;
                snprintf(
                    out_buf,
                    out_sz,
                    "%s",
                    rel
                );
                return;
            }
        }
    }

    const char *p = norm_path;
    while (*p == '/') p++;
    snprintf(
        out_buf,
        out_sz,
        "%s",
        p
    );
}

bool scandir_execute(
    const char *path,
    const char *clean_filter
) {
    char real_path[PATH_MAX];
    if (!realpath(path, real_path)) {
        return false;
    }

    struct stat st;
    if (
        stat(real_path, &st) != 0 ||
        !S_ISDIR(st.st_mode)
    ) {
        return false;
    }

    if (is_visited(st.st_dev, st.st_ino)) {
        return false;
    }
    mark_visited(st.st_dev, st.st_ino);

    char clean_p[PATH_MAX];
    get_clean_path(
        path,
        clean_p,
        sizeof(clean_p)
    );

    bool match = true;
    const char *next_filter = clean_filter;

    if (
        clean_filter &&
        strlen(clean_filter) > 0
    ) {
        if (starts_with(clean_p, clean_filter)) {
            match = true;
            next_filter = NULL;
        } else if (starts_with(clean_filter, clean_p)) {
            match = false;
            next_filter = clean_filter;
        } else {
            return false;
        }
    }

    DIR *dir = opendir(real_path);
    if (!dir) return false;

    char **executables = NULL;
    size_t exec_count = 0;
    
    char **subdirs = NULL;
    size_t sub_count = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (
            strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0
        ) {
            continue;
        }

        char full_path[PATH_MAX];
        snprintf(
            full_path,
            sizeof(full_path),
            "%s/%s",
            real_path,
            entry->d_name
        );

        struct stat ent_st;
        if (stat(full_path, &ent_st) == 0) {
            if (
                S_ISREG(ent_st.st_mode) &&
                (access(full_path, X_OK) == 0)
            ) {
                executables = realloc(
                    executables,
                    (exec_count + 1) * sizeof(char *)
                );
                executables[exec_count++] = strdup(entry->d_name);
            } else if (S_ISDIR(ent_st.st_mode)) {
                subdirs = realloc(
                    subdirs,
                    (sub_count + 1) * sizeof(char *)
                );
                subdirs[sub_count++] = strdup(full_path);
            }
        }
    }
    closedir(dir);

    bool found = false;

    if (exec_count > 0 && match) {
        qsort(
            executables,
            exec_count,
            sizeof(char *),
            compare_strings
        );

        printf(
            "%s%s:\n",
            color_N, clean_p
        );

        for (size_t i = 0; i < exec_count; i++) {
            printf(
                "%s› %s%s%s\n",
                color_R, color_GG, executables[i], color_N
            );
        }

        printf("\n");
        found = true;
    }

    qsort(
        subdirs,
        sub_count,
        sizeof(char *),
        compare_strings
    );

    for (size_t i = 0; i < sub_count; i++) {
        if (scandir_execute(subdirs[i], next_filter)) {
            found = true;
        }
    }

    for (size_t i = 0; i < exec_count; i++) {
        free(executables[i]);
    }
    free(executables);

    for (size_t i = 0; i < sub_count; i++) {
        free(subdirs[i]);
    }
    free(subdirs);

    return found;
}

int main(int argc, char *argv[]) {
    bool allpath = false;
    char *only = NULL;
    bool valid = true;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--allpath") == 0) {
            allpath = true;
        } else if (strcmp(argv[i], "--only") == 0) {
            if (i + 1 < argc) {
                only = argv[i + 1];
                i++;
            } else {
                valid = false;
                break;
            }
        } else {
            invalid_option(argv[i], "atoolkit");
            return 1;
        }
    }

    if (!valid) {
        missing_argument("atoolkit");
        return 1;
    }

    const char *source_env = allpath ?
        getenv("PATH") :
        getenv("__bin__");

    if (!source_env && !allpath) {
        source_env = getenv("PATH");
    }

    if (!source_env || strlen(source_env) == 0) {
        fprintf(
            stderr,
            "%s[!] %sEnvironment variable not found!\n",
            color_R, color_N
        );
        return 1;
    }

    char *clean_filter = NULL;
    if (only) {
        clean_filter = strdup(only);
        clean_filter = strip_slashes(clean_filter);
    }

    bool found_any = false;

    char *source_dup = strdup(source_env);
    char *saveptr;
    char *p = strtok_r(
        source_dup,
        ":",
        &saveptr
    );
    
    while (p != NULL) {
        if (strlen(p) > 0) {
            if (scandir_execute(p, clean_filter)) {
                found_any = true;
            }
        }
        p = strtok_r(
            NULL,
            ":",
            &saveptr
        );
    }
    free(source_dup);

    if (clean_filter && !found_any) {
        fprintf(
            stderr,
            "%s[!] %sPath: %s%s %snot found!\n", 
            color_R, color_N, color_GG, only, color_N
        );
    }

    if (clean_filter) {
        free(clean_filter);
    }

    if (visited_dirs) {
        free(visited_dirs);
    }

    return 0;
}

// Copyright (c) 2026 Zeronetsec