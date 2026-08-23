// https://github.com/Zeronetsec/Ares

// Usage: fchmod <target> --mode <mod> [--only <file|dir|symlink>] [--ext <.ext>]

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>

#include <color.h>
#include <missing_argument.h>
#include <invalid_option.h>

typedef struct {
    mode_t mode;
    bool mode_set;
    bool only_file;
    bool only_dir;
    bool only_symlink;
    char ext[256];
    bool has_ext;
} Options;

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
        suffix, len_suffix
    ) == 0;
}

bool should_chmod(
    const char *filepath,
    struct stat *st,
    Options *opts
) {
    if (S_ISDIR(st->st_mode)) {
        return opts->only_dir;
    } else if (S_ISREG(st->st_mode)) {
        if (!opts->only_file) {
            return false;
        }

        if (
            opts->has_ext &&
            !ends_with(filepath, opts->ext)
        ) {
            return false;
        }
        return true;
    } else if (S_ISLNK(st->st_mode)) {
        if (!opts->only_symlink) {
            return false;
        }

        if (
            opts->has_ext &&
            !ends_with(filepath, opts->ext)
        ) {
            return false;
        }
        return true;
    }
    return false;
}

void process_path(const char *path, Options *opts) {
    struct stat st;
    if (lstat(path, &st) == -1) {
        perror(path);
        return;
    }

    if (should_chmod(path, &st, opts)) {
        if (chmod(path, opts->mode) == 0) {
            printf(
                "%s[+] %sSet mode: %s%04o %s-> %s%s%s\n",
                GG, N, GG, opts->mode, DG, GG, path, N
            );
        } else {
            perror(path);
        }
    }

    if (S_ISDIR(st.st_mode)) {
        DIR *dir = opendir(path);
        if (!dir) {
            perror(path);
            return;
        }

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
            process_path(next_path, opts);
        }
        closedir(dir);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        missing_argument("fchmod");
        return 1;
    }

    Options opts = {0};
    char *target = NULL;
    bool only_flag_seen = false;

    opts.only_file = true;
    opts.only_dir = true;
    opts.only_symlink = true;

    for (int i = 1; i < argc; i++) {
        if (
            strcmp(argv[i], "--mode") == 0 &&
            i + 1 < argc
        ) {
            opts.mode = strtol(argv[++i], NULL, 8);
            opts.mode_set = true;
        } else if (
            strcmp(argv[i], "--only") == 0 &&
            i + 1 < argc
        ) {
            if (!only_flag_seen) {
                opts.only_file = opts.only_dir = opts.only_symlink = false;
                only_flag_seen = true;
            }
            i++;
            if (strcmp(argv[i], "file") == 0) {
                opts.only_file = true;
            } else if (strcmp(argv[i], "dir") == 0) {
                opts.only_dir = true;
            } else if (strcmp(argv[i], "symlink") == 0) {
                opts.only_symlink = true;
            } else {
                char invalid_args[256]; 
                snprintf(
                    invalid_args,
                    sizeof(invalid_args),
                    "--only %s",
                    argv[i]
                );

                invalid_option(
                    invalid_args,
                    "fchmod"
                );
                return 1;
            }
        } else if (
            strcmp(argv[i], "--ext") == 0 &&
            i + 1 < argc
        ) {
            strncpy(
                opts.ext,
                argv[++i],
                sizeof(opts.ext) - 1
            );
            opts.has_ext = true;
        } else if (target == NULL) {
            target = argv[i];
        }
    }

    if (!target || !opts.mode_set) {
        missing_argument("fchmod");
        return 1;
    }

    char *target_copy = strdup(target);
    char *token = strtok(target_copy, ":");

    while (token != NULL) {
        if (strlen(token) > 0) {
            printf(
                "%s[*] %sProcessing: %s%s%s\n",
                B, N, GG, token, N
            );
            process_path(token, &opts);
        }
        token = strtok(NULL, ":");
    }

    free(target_copy);
    return 0;
}

// Copyright (c) 2026 Zeronetsec