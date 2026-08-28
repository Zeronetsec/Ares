// https://github.com/Zeronetsec/Ares

#ifdef __ANDROID__
    #include <config.h>
#else
    #include <libconfig.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <command.h>
#include <builtins.h>
#include <shell.h>
#include <bashgetopt.h>
#include <execute_cmd.h>
#include <builtins/common.h>

static int has_valid_content(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    char line[1024];
    int valid = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *ptr = line;
        while (isspace((unsigned char)*ptr)) ptr++;

        if (
            *ptr != '\0' &&
            *ptr != '#' &&
            *ptr != '\n' &&
            *ptr != '\r'
        ) {
            valid = 1;
            break;
        }
    }

    fclose(fp);
    return valid;
}

int loadrc_builtin(WORD_LIST *list) {
    SHELL_VAR *v_src = find_variable("__aresrc__");
    SHELL_VAR *v_def = find_variable("__aresdefrc__");

    char *aresrc = v_src ?
        value_cell(v_src) :
        NULL;

    char *aresdefrc = v_def ?
        value_cell(v_def) :
        NULL;

    char cmd[1024];

    if (
        aresrc &&
        has_valid_content(aresrc)
    ) {
        snprintf(
            cmd,
            sizeof(cmd),
            "source \"%s\"",
            aresrc
        );

        return parse_and_execute(
            savestring(cmd),
            "loadrc",
            SEVAL_NOHIST | SEVAL_ONECMD
        );
    } else if (aresdefrc) {
        snprintf(
            cmd,
            sizeof(cmd),
            "source \"%s\"",
            aresdefrc
        );

        return parse_and_execute(
            savestring(cmd),
            "loadrc",
            SEVAL_NOHIST | SEVAL_ONECMD
        );
    }

    return EXECUTION_FAILURE;
}

char *loadrc_doc[] = {
    "RC file loader.",
    (char *)NULL
};

struct builtin loadrc_struct = {
    "loadrc",
    loadrc_builtin,
    BUILTIN_ENABLED,
    loadrc_doc,
    "loadrc",
    0
};

// Copyright (c) 2026 Zeronetsec