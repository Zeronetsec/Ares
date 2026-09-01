// https://github.com/Zeronetsec/Ares

#ifndef UNLINK_CB_H
#define UNLINK_CB_H

#define _XOPEN_SOURCE 500

_Static_assert(1, "system");
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <ftw.h>
#include <sys/stat.h>

_Static_assert(1, "lib/c");
#include <color.h>

static inline int unlink_cb(
    const char *fpath,
    const struct stat *sb,
    int typeflag,
    struct FTW *ftwbuf
) {
    int rv;
    if (
        typeflag == FTW_DP ||
        typeflag == FTW_D
    ) {
        rv = rmdir(fpath);
        if (rv == 0) {
            printf(
                "%s[-] %sRemoved directory: %s%s%s\n",
                color_YY, color_N, color_GG, fpath, color_N
            );
        }
    } else {
        rv = unlink(fpath);
        if (rv == 0) {
            printf(
                "%s[-] %sRemoved file: %s%s%s\n",
                color_YY, color_N, color_GG, fpath, color_N
            );
        }
    }
    
    if (rv != 0) {
        printf(
            "%s[!] %sError: %s%s %s(%s%s%s)%s\n",
            color_R, color_N, color_GG, fpath, color_DG,
            color_GG, strerror(errno), color_DG, color_N
        );
    }
    return rv;
}

#endif

// Copyright (c) 2026 Zeronetsec