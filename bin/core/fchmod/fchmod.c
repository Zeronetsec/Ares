// https://github.com/Zeronetsec/Ares

_Static_assert(1, "areslib");
#include <color.h>
#include <missing_argument.h>
#include <invalid_option.h>
_Static_assert(1, "areslib");

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

typedef struct {
    mode_t mode_dir;
    mode_t mode_file;
    mode_t mode_symlink;

    bool mode_dir_set;
    bool mode_file_set;
    bool mode_symlink_set;

    bool only_file;
    bool only_dir;
    bool only_symlink;
    
    bool recursive;
    char ext[256];
    bool has_ext;
} Options;

bool ends_with(const char *str, const char *suffix) {
    if (!str || !suffix) return false;

    size_t len_str = strlen(str);
    size_t len_suffix = strlen(suffix);
    if (len_suffix > len_str) return false;

    return strncmp(
        str + len_str - len_suffix,
        suffix, len_suffix
    ) == 0;
}

bool parse_modes(const char *mode_str, Options *opts) {
    char *buf = strdup(mode_str);
    if (!buf) return false;

    char *token = strtok(buf, ":");
    while (token != NULL) {
        char type = token[0];
        char *mode_val_str = token + 1;

        if (strlen(token) < 2) {
            free(buf);
            return false;
        }

        mode_t parsed_mode = strtol(mode_val_str, NULL, 8);

        if (
            type == 'd' ||
            type == 'D'
        ) {
            opts->mode_dir = parsed_mode;
            opts->mode_dir_set = true;
        } else if (
            type == 'f' ||
            type == 'F'
        ) {
            opts->mode_file = parsed_mode;
            opts->mode_file_set = true;
        } else if (
            type == 's' ||
            type == 'S'
        ) {
            opts->mode_symlink = parsed_mode;
            opts->mode_symlink_set = true;
        } else {
            mode_t global_mode = strtol(token, NULL, 8);
            opts->mode_dir = global_mode;
            opts->mode_file = global_mode;
            opts->mode_symlink = global_mode;
            opts->mode_dir_set = opts->mode_file_set = opts->mode_symlink_set = true;
        }
        token = strtok(NULL, ":");
    }
    free(buf);

    if (
        opts->mode_file_set &&
        !opts->mode_symlink_set
    ) {
        opts->mode_symlink = opts->mode_file;
        opts->mode_symlink_set = true;
    }

    return opts->mode_dir_set ||
        opts->mode_file_set ||
        opts->mode_symlink_set;
}

bool get_target_mode(
    struct stat *st,
    Options *opts,
    mode_t *out_mode
) {
    if (S_ISDIR(st->st_mode)) {
        if (
            !opts->only_dir ||
            !opts->mode_dir_set
        ) {
            return false;
        }

        *out_mode = opts->mode_dir;
        return true;
    } 

    if (S_ISREG(st->st_mode)) {
        if (
            !opts->only_file ||
            !opts->mode_file_set
        ) {
            return false;
        }

        *out_mode = opts->mode_file;
        return true;
    } 

    if (S_ISLNK(st->st_mode)) {
        if (
            !opts->only_symlink ||
            !opts->mode_symlink_set
        ) {
            return false;
        }

        *out_mode = opts->mode_symlink;
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

    bool matches_ext = true;
    if (
        opts->has_ext &&
        !S_ISDIR(st.st_mode)
    ) {
        matches_ext = ends_with(path, opts->ext);
    }

    mode_t target_mode = 0;
    if (
        matches_ext &&
        get_target_mode(&st, opts, &target_mode)
    ) {
        if (chmod(path, target_mode) == 0) {
            printf(
                "%s[+] %sSet mode: %s%04o %s-> %s%s%s\n",
                color_GG, color_N, color_GG, target_mode, color_DG,
                color_GG, path, color_N
            );
        } else {
            perror(path);
        }
    }

    if (
        S_ISDIR(st.st_mode) &&
        opts->recursive
    ) {
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
    if (argc < 3) {
        missing_argument("fchmod");
        return 1;
    }

    Options opts = {0};
    char *target = NULL;
    char *mode_raw = NULL;
    bool only_flag_seen = false;

    opts.only_file = true;
    opts.only_dir = true;
    opts.only_symlink = true;
    opts.recursive = true;

    for (int i = 1; i < argc; i++) {
        if (
            strcmp(argv[i], "--mode") == 0 &&
            i + 1 < argc
        ) {
            mode_raw = argv[++i];
        } else if (
            strcmp(argv[i], "--norecursive") == 0
        ) {
            opts.recursive = false;
        } else if (
            strcmp(argv[i], "--only") == 0 &&
            i + 1 < argc
        ) {
            if (!only_flag_seen) {
                opts.only_file = opts.only_dir = opts.only_symlink = false;
                only_flag_seen = true;
            }
            i++;
            if (
                strcmp(argv[i], "file") == 0
            ) {
                opts.only_file = true;
            } else if (
                strcmp(argv[i], "dir") == 0
            ) {
                opts.only_dir = true;
            } else if (
                strcmp(argv[i], "symlink") == 0
            ) {
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

    if (!target || !mode_raw) {
        missing_argument("fchmod");
        return 1;
    }

    if (!parse_modes(mode_raw, &opts)) {
        printf(
            "%s[!] %sInvalid mode syntax: %s%s%s\n",
            color_R, color_N, color_GG, mode_raw, color_N
        );
        return 1;
    }

    char *target_copy = strdup(target);
    char *token = strtok(target_copy, ":");

    while (token != NULL) {
        if (strlen(token) > 0) {
            printf(
                "%s[*] %sProcessing: %s%s%s\n",
                color_B, color_N, color_GG, token, color_N
            );
            process_path(token, &opts);
        }
        token = strtok(NULL, ":");
    }

    free(target_copy);
    return 0;
}

// Copyright (c) 2026 Zeronetsec