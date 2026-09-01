// https://github.com/Zeronetsec/Ares

_Static_assert(1, "system");
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

_Static_assert(1, "lib/c");
#include <color.h>
#include <missing_argument.h>

_Static_assert(1, "bin/core/mklog");
#include <mkdir_p.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        missing_argument("mklog");
        return 1;
    }

    const char *fileArg = argv[1];
    const char *tagArg = argv[2];

    char logMsg[8192] = {0}; 
    for (int i = 3; i < argc; i++) {
        strcat(logMsg, argv[i]);
        if (i < argc - 1) {
            strcat(logMsg, " ");
        }
    }

    const char *aresLogDir = getenv("__areslog__");
    if (!aresLogDir) {
        aresLogDir = "";
    }

    char baseDir[1024];
    snprintf(
        baseDir,
        sizeof(baseDir),
        "%s/%s",
        aresLogDir,
        fileArg
    );

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char timeStr[64];
    char logTimeStr[32];

    strftime(
        timeStr,
        sizeof(timeStr),
        "%Y_%B_%d",
        tm_info
    );

    strftime(
        logTimeStr,
        sizeof(logTimeStr),
        "%H:%M",
        tm_info
    );

    const char *prefix;
    if (
        strcmp(tagArg, ":") == 0
    ) {
        prefix = "[+]";
    } else if (
        strcmp(tagArg, ":error") == 0
    ) {
        prefix = "[!]";
    } else if (
        strcmp(tagArg, ":info") == 0
    ) {
        prefix = "[*]";
    } else if (
        strcmp(tagArg, ":?") == 0
    ) {
        prefix = "[?]";
    } else if (
        strcmp(tagArg, ":-") == 0
    ) {
        prefix = "[-]";
    } else {
        prefix = "[ ]";
    }

    mkdir_p(baseDir);
    struct stat st = {0};
    if (stat(baseDir, &st) == -1) {
        printf(
            "%s[!] %sError create directory: %s%s%s\n",
            color_R, color_N, color_GG, strerror(errno), color_N
        );
        return 1;
    }

    char finalLogPath[2048];
    snprintf(
        finalLogPath,
        sizeof(finalLogPath),
        "%s/%s_%s.log",
        baseDir, fileArg, timeStr
    );

    char fullLogLine[8192];
    snprintf(
        fullLogLine,
        sizeof(fullLogLine),
        "%s - %s %s\n",
        logTimeStr, prefix, logMsg
    );

    FILE *f = fopen(finalLogPath, "a");
    if (f == NULL) {
        printf(
            "%s[!] %sError open/create file: %s%s%s\n",
            color_R, color_N, color_GG, strerror(errno), color_N
        );
        return 1;
    }

    if (fputs(fullLogLine, f) == EOF) {
        printf(
            "%s[!] %sError writing log: %s%s%s\n",
            color_R, color_N, color_GG, strerror(errno), color_N
        );
        fclose(f);
        return 1;
    }

    fclose(f);
    return 0;
}

// Copyright (c) 2026 Zeronetsec