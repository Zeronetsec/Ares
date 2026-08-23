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
#include <builtins.h>
#include <shell.h>
#include <bashgetopt.h>

static char *trim_whitespace(char *str) {
    char *end;
    while (
        *str == ' ' ||
        *str == '\t'
    ) {
        str++;
    }

    if (*str == 0) {
        return str;
    }

    end = str + strlen(str) - 1;
    while (
        end > str && (
            *end == ' ' ||
            *end == '\t' ||
            *end == '\n' ||
            *end == '\r'
        )
    ) {
        end--;
    }

    end[1] = '\0';
    return str;
}

int rhome_builtin(WORD_LIST *list) {
    char *config_var = get_string_value("__config__");
    char *orig_home = get_string_value("HOME");
    char *new_home = NULL;
    char filepath[1024];

    if (config_var && *config_var) {
        snprintf(
            filepath,
            sizeof(filepath),
            "%s/startup.conf",
            config_var
        );

        FILE *fp = fopen(filepath, "r");
        if (fp) {
            char line[1024];
            while (fgets(line, sizeof(line), fp)) {
                char *trimmed = trim_whitespace(line);
                if (strncmp(trimmed, "home", 4) == 0) {
                    char *p = trimmed + 4;

                    while (*p == ' ' || *p == '\t') p++;
                    if (*p == '=') {
                        p++;
                        while (*p == ' ' || *p == '\t') p++;
                        if (*p != '\0') {
                            new_home = strdup(p);
                        }
                        break;
                    }
                }
            }
            fclose(fp);
        }
    }

    char *final_home = NULL;

    if (new_home) {
        if (
            strcmp(new_home, "~") == 0 || 
            strcmp(new_home, "${HOME}") == 0 || 
            strcmp(new_home, "$HOME") == 0
        ) {
            final_home = orig_home ?
                strdup(orig_home) :
                strdup("/");
        } else if (
            strncmp(new_home, "~/", 2) == 0 &&
            orig_home
        ) {
            final_home = malloc(
                strlen(orig_home) + strlen(new_home)
            );

            sprintf(
                final_home,
                "%s/%s",
                orig_home, new_home + 2
            );
        } else {
            final_home = strdup(new_home);
        }
        free(new_home);
    } else {
        final_home = orig_home ?
            strdup(orig_home) :
            strdup("/");
    }

    SHELL_VAR *v = bind_variable(
        "HOME",
        final_home,
        0
    );

    if (v) {
        VSETATTR(
            v,
            att_exported
        );
    }

    free(final_home);
    return EXECUTION_SUCCESS;
}

char *rhome_doc[] = {
    "Dynamic $HOME resolver.",
    (char *)NULL
};

struct builtin rhome_struct = {
    "rhome",
    rhome_builtin,
    BUILTIN_ENABLED,
    rhome_doc,
    "rhome",
    0
};

// Copyright (c) 2026 Zeronetsec