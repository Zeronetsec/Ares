// https://github.com/Zeronetsec/Ares

#ifdef __ANDROID__
    #include <config.h>
#else
    #include <libconfig.h>
#endif

#include <color.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <builtins.h>
#include <shell.h>
#include <sys/stat.h>
#include <builtins/common.h>

extern int source_file (const char *, int);

int is_valid_sh(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) return 0;

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        char *p = line;

        while (*p && isspace(*p)) p++;
        if (
            *p != '\0' &&
            *p != '\n' &&
            *p != '#'
        ) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

int has_valid_sh_files(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (!dir) return 0;

    struct dirent *entry;
    char path[4096];
    int found_valid = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (
            strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0
        ) {
            continue;
        }

        snprintf(
            path,
            sizeof(path),
            "%s/%s",
            dir_path,
            entry->d_name
        );

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) {
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            if (has_valid_sh_files(path)) {
                found_valid = 1;
                break;
            }
        } else if (S_ISREG(statbuf.st_mode)) {
            size_t len = strlen(entry->d_name);
            if (
                len > 3 &&
                strcmp(entry->d_name + len - 3, ".sh") == 0
            ) {
                if (is_valid_sh(path)) {
                    found_valid = 1;
                    break;
                }
            }
        }
    }
    closedir(dir);
    return found_valid;
}

void scan_and_source(const char *dir_path) {
    DIR *dir = opendir(dir_path);
    if (!dir) return;

    struct dirent *entry;
    char path[4096];

    while ((entry = readdir(dir)) != NULL) {
        if (
            strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0
        ) {
            continue;
        }

        snprintf(
            path,
            sizeof(path),
            "%s/%s",
            dir_path,
            entry->d_name
        );

        struct stat statbuf;
        if (stat(path, &statbuf) == -1) {
            continue;
        }

        if (S_ISDIR(statbuf.st_mode)) {
            scan_and_source(path); 
        } else if (S_ISREG(statbuf.st_mode)) {
            size_t len = strlen(entry->d_name);
            if (
                len > 3 &&
                strcmp(entry->d_name + len - 3, ".sh") == 0
            ) {
                if (is_valid_sh(path)) {
                    source_file(path, 0);
                }
            }
        }
    }
    closedir(dir);
}

int shmod_builtin(WORD_LIST *list) {
    int using_user_shmod = 0;

    char *shmoduser = get_string_value("__shmoduser__");
    if (
        shmoduser != NULL &&
        has_valid_sh_files(shmoduser)
    ) {
        scan_and_source(shmoduser);
        using_user_shmod = 1;
    }

    if (!using_user_shmod) {
        char *init = get_string_value("__init__");
        if (!init) {
            fprintf(
                stderr,
                "%s[!] %sShmod: %s__init__ %snot found!\n",
                R, N, GG, N
            );
            return EXECUTION_FAILURE;
        }

        char target_dir[4096];
        snprintf(
            target_dir,
            sizeof(target_dir),
            "%s/shmod",
            init
        );

        scan_and_source(target_dir);
    }

    return EXECUTION_SUCCESS;
}

char *shmod_doc[] = {
    "Load modular shell script.",
    (char *)NULL
};

struct builtin shmod_struct = {
    "shmod",
    shmod_builtin,
    BUILTIN_ENABLED,
    shmod_doc,
    "shmod",
    0
};

// Copyright (c) 2026 Zeronetsec