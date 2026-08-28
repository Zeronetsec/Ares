// https://github.com/Zeronetsec/Ares

_Static_assert(1, "areslib");
#include <color.h>
_Static_assert(1, "areslib");

#ifdef __ANDROID__
    #include <config.h>
#else
    #include <libconfig.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <builtins.h>
#include <shell.h>
#include <builtins/common.h>

int llsi_builtin(WORD_LIST *list) {
    char *aresroot = get_string_value("__aresroot__");
    if (!aresroot) {
        fprintf(
            stderr,
            "%s[!] %sLlsi: variable %s__aresroot__ %snot founf!\n",
            R, N, GG, N
        );
        return EXECUTION_FAILURE;
    }

    char init_file[4096];
    snprintf(
        init_file,
        sizeof(init_file),
        "%s/init/llsi.init",
        aresroot
    );

    FILE *fp = fopen(init_file, "r");
    if (!fp) {
        fprintf(
            stderr,
            "%s[!] %sLlsi: failed to open %s%s%s\n",
            R, N, GG, init_file, N
        );
        return EXECUTION_FAILURE;
    }

    char line[1024];
    char current_block[256] = "";
    int in_block = 0;

    while (
        fgets(
            line,
            sizeof(line),
            fp
        )
    ) {
        char *ptr = line;
        while (
            *ptr == ' ' ||
            *ptr == '\t' ||
            *ptr == '\n' ||
            *ptr == '\r'
        ) {
            ptr++;
        }

        if (
            *ptr == '\0' ||
            *ptr == '#'
        ) continue;

        if (*ptr == '}') {
            in_block = 0;
            current_block[0] = '\0';
            continue;
        }

        char *brace = strchr(ptr, '{');
        if (brace) {
            *brace = '\0';
            char *end = brace - 1;
            while (
                end >= ptr &&
                (
                    *end == ' ' ||
                    *end == '\t'
                )
            ) {
                *end-- = '\0';
            }

            strncpy(
                current_block,
                ptr,
                sizeof(current_block) - 1
            );

            current_block[sizeof(current_block) - 1] = '\0';
            in_block = 1;
            continue;
        }

        if (in_block) {
            char *arrow = strstr(ptr, "->");
            if (arrow) {
                *arrow = '\0';
                char *key = ptr;
                char *val = arrow + 2;

                char *end = arrow - 1;
                while (
                    end >= key &&
                    (
                        *end == ' ' ||
                        *end == '\t'
                    )
                ) {
                    *end-- = '\0';
                }

                while (
                    *val == ' ' ||
                    *val == '\t'
                ) {
                    val++;
                }

                char *vend = val + strlen(val) - 1;
                while (
                    vend >= val &&
                    (
                        *vend == ' ' ||
                        *vend == '\t' ||
                        *vend == '\n' ||
                        *vend == '\r'
                    )
                ) {
                    *vend-- = '\0';
                }

                char target_path[1024];
                if (
                    aresroot[strlen(aresroot) - 1] == '/' ||
                    val[0] == '/'
                ) {
                    snprintf(
                        target_path,
                        sizeof(target_path),
                        "%s%s",
                        aresroot,
                        val
                    );
                } else {
                    snprintf(
                        target_path,
                        sizeof(target_path),
                        "%s/%s",
                        aresroot,
                        val
                    );
                }

                struct stat st;
                if (stat(target_path, &st) == 0) {
                    char cmd[1024];
                    snprintf(
                        cmd,
                        sizeof(cmd),
                        "source \"%s\"",
                        target_path
                    );

                    evalstring(
                        cmd,
                        "llsi_builtin",
                        0
                    );
                } else {
                    printf(
                        "%s[!] %sLlsi: %s%s %sfrom %s%s %sin %s%s %snot found!\n",
                        R, N, GG, val, N,
                        CC, current_block, N,
                        YY, key, N
                    );
                }
            }
        }
    }

    fclose(fp);
    return EXECUTION_SUCCESS;
}

char *llsi_doc[] = {
    "Loads lib shell init.",
    (char *)NULL
};

struct builtin llsi_struct = {
    "llsi",
    llsi_builtin,
    BUILTIN_ENABLED,
    llsi_doc,
    "llsi",
    0
};

// Copyright (c) 2026 Zeronetsec