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
#include <limits.h>
#include <builtins.h>
#include <shell.h>
#include <bashgetopt.h>

typedef struct {
    char *str;
    size_t len;
    size_t capacity;
} StringBuffer;

static void append_path(
    StringBuffer *sb,
    const char *path
) {
    if (!path || !*path) {
        return;
    }

    size_t plen = strlen(path);
    if (
        sb->len + plen + 2 > sb->capacity
    ) {
        sb->capacity = (sb->capacity + plen + 1024) * 2;
        char *new_str = realloc(
            sb->str,
            sb->capacity
        );
        if (!new_str) return;
        sb->str = new_str;
    }

    if (sb->len > 0) {
        sb->str[sb->len++] = ':';
    }
    
    strcpy(
        sb->str + sb->len,
        path
    );
    sb->len += plen;
}

static void traverse_and_find_dirs(
    const char *dirpath,
    StringBuffer *sb
) {
    DIR *dir = opendir(dirpath);
    if (!dir) {
        return;
    }

    append_path(sb, dirpath);

    struct dirent *entry;
    char childpath[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        if (
            strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0
        ) {
            continue;
        }

        snprintf(
            childpath,
            sizeof(childpath),
            "%s/%s",
            dirpath,
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
                stat(childpath, &st) == 0 &&
                S_ISDIR(st.st_mode)
            ) {
                is_dir = 1;
            }
        }

        if (is_dir) {
            traverse_and_find_dirs(
                childpath,
                sb
            );
        }
    }
    closedir(dir);
}

int dynap_builtin(WORD_LIST *list) {
    StringBuffer new_path = {NULL, 0, 0};

    char *aresroot = get_string_value("__aresroot__");
    if (aresroot && *aresroot) {
        char aresbin[PATH_MAX];
        snprintf(
            aresbin,
            sizeof(aresbin),
            "%s/bin",
            aresroot
        );

        traverse_and_find_dirs(
            aresbin,
            &new_path
        );
    }

    char *curr_path = get_string_value("PATH");
    if (curr_path && *curr_path) {
        append_path(
            &new_path,
            curr_path
        );
    }

    char *home = get_string_value("HOME");
    if (home && *home) {
        char go_bin[PATH_MAX];
        snprintf(
            go_bin,
            sizeof(go_bin),
            "%s/go/bin",
            home
        );

        struct stat st;
        if (
            stat(go_bin, &st) == 0 &&
            S_ISDIR(st.st_mode)
        ) {
            append_path(
                &new_path,
                go_bin
            );
        }
    }

    if (new_path.str) {
        SHELL_VAR *v = bind_variable(
            "PATH",
            new_path.str,
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

        free(new_path.str);
    }

    return EXECUTION_SUCCESS;
}

char *dynap_doc[] = {
    "Dynamic $PATH generator.",
    (char *)NULL
};

struct builtin dynap_struct = {
    "dynap",
    dynap_builtin,
    BUILTIN_ENABLED,
    dynap_doc,
    "dynap",
    0
};

// Copyright (c) 2026 Zeronetsec