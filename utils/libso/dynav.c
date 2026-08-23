// https://github.com/Zeronetsec/Ares

#ifdef __ANDROID__
    #include <config.h>
#else
    #include <libconfig.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <limits.h>
#include <builtins.h>
#include <shell.h>
#include <bashgetopt.h>

int dynav_builtin(WORD_LIST *list) {
    char *aresroot = get_string_value("__aresroot__");
    if (!aresroot || !*aresroot) {
        return EXECUTION_SUCCESS;
    }

    char base_path[PATH_MAX];
    strncpy(
        base_path,
        aresroot,
        sizeof(base_path) - 1
    );
    base_path[sizeof(base_path) - 1] = '\0';

    size_t len = strlen(base_path);
    while (len > 0 && base_path[len - 1] == '/') {
        base_path[len - 1] = '\0';
        len--;
    }

    DIR *dir = opendir(base_path);
    if (!dir) {
        return EXECUTION_SUCCESS;
    }

    const char *ignored_folders[] = {
        ".git",
        ".install",
        ".github",
        ".vscode",
        ".laction",
        ".docs",
        NULL 
    };

    struct dirent *entry;
    char fullpath[PATH_MAX];
    char var_name[NAME_MAX + 10];

    while ((entry = readdir(dir)) != NULL) {
        if (
            strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0
        ) {
            continue;
        }

        int skip = 0;
        for (int i = 0; ignored_folders[i] != NULL; i++) {
            if (
                strcmp(
                    entry->d_name,
                    ignored_folders[i]
                ) == 0
            ) {
                skip = 1;
                break;
            }
        }

        if (skip) {
            continue;
        }

        snprintf(
            fullpath,
            sizeof(fullpath),
            "%s/%s",
            base_path,
            entry->d_name
        );

        int is_dir = 0;
        #ifdef _DIRENT_HAVE_D_TYPE
            if (entry->d_type != DT_UNKNOWN) {
                is_dir = (entry->d_type == DT_DIR);
            } else
        #endif
        {
            struct stat st;
            if (
                stat(fullpath, &st) == 0 &&
                S_ISDIR(st.st_mode)
            ) {
                is_dir = 1;
            }
        }

        if (!is_dir) continue;

        char lower_name[NAME_MAX];
        strncpy(
            lower_name,
            entry->d_name,
            sizeof(lower_name) - 1
        );
        lower_name[sizeof(lower_name) - 1] = '\0';
        
        for (int i = 0; lower_name[i]; i++) {
            lower_name[i] = tolower(
                (unsigned char)lower_name[i]
            );
        }

        snprintf(
            var_name,
            sizeof(var_name),
            "__%s__",
            lower_name
        );

        SHELL_VAR *v = bind_variable(
            var_name,
            fullpath,
            0
        );

        if (v) {
            VSETATTR(
                v,
                att_exported
            );

            VSETATTR(
                v,
                att_readonly
            );
        }
    }

    closedir(dir);
    return EXECUTION_SUCCESS;
}


char *dynav_doc[] = {
    "Dynamic $__var__ exporter.",
    (char *)NULL
};

struct builtin dynav_struct = {
    "dynav",
    dynav_builtin,
    BUILTIN_ENABLED,
    dynav_doc,
    "dynav",
    0
};

// Copyright (c) 2026 Zeronetsec