// https://github.com/Zeronetsec/Ares

#ifndef GET_CLEAN_PATH_H
#define GET_CLEAN_PATH_H

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>

_Static_assert(1, "bin/core/atoolkit");
#include <starts_with.h>
#include <rstrip_slash.h>

static inline void get_clean_path(
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

#endif

// Copyright (c) 2026 Zeronetsec