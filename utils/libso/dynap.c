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
#include <limits.h>
#include <builtins.h>
#include <shell.h>
#include <bashgetopt.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct {
    char *str;
    size_t len;
    size_t capacity;
} StringBuffer;

static char *trim_whitespace(char *str) {
    while (
        *str == ' ' ||
        *str == '\t' ||
        *str == '\r' ||
        *str == '\n'
    ) {
        str++;
    }

    if (*str == 0) return str;
    char *end = str + strlen(str) - 1;
    while (
        end > str &&
        (
            *end == ' ' ||
            *end == '\t' ||
            *end == '\r' ||
            *end == '\n'
        )
    ) {
        *end = '\0';
        end--;
    }
    return str;
}

static int path_contains(
    const char *path_list,
    const char *path
) {
    if (
        !path_list ||
        !*path_list ||
        !path ||
        !*path
    ) return 0;

    size_t plen = strlen(path);
    while (
        plen > 1 &&
        path[plen - 1] == '/'
    ) plen--;

    const char *p = path_list;
    while (*p) {
        const char *end = strchr(p, ':');
        size_t token_len = end ?
            (size_t)(end - p) :
            strlen(p);

        size_t clean_token_len = token_len;
        while (
            clean_token_len > 1 &&
            p[clean_token_len - 1] == '/'
        ) {
            clean_token_len--;
        }

        if (
            clean_token_len == plen &&
            strncmp(p, path, plen) == 0
        ) {
            return 1;
        }

        if (!end) break;
        p = end + 1;
    }

    return 0;
}

static void append_path(
    StringBuffer *sb,
    const char *path
) {
    if (!path || !*path) return;

    char clean_path[PATH_MAX];
    strncpy(
        clean_path,
        path,
        sizeof(clean_path) - 1
    );
    clean_path[sizeof(clean_path) - 1] = '\0';

    size_t plen = strlen(clean_path);
    while (
        plen > 1 &&
        clean_path[plen - 1] == '/'
    ) {
        clean_path[--plen] = '\0';
    }

    if (
        sb->str &&
        path_contains(sb->str, clean_path)
    ) {
        return;
    }

    if (sb->len + plen + 2 > sb->capacity) {
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
        clean_path
    );
    sb->len += plen;
}

static void append_path_list(
    StringBuffer *sb,
    const char *path_list
) {
    if (!path_list || !*path_list) return;

    char *dup = strdup(path_list);
    if (!dup) return;

    char *token = strtok(dup, ":");
    while (token) {
        append_path(sb, token);
        token = strtok(NULL, ":");
    }
    free(dup);
}

static void expand_vars(
    const char *src,
    char *dest,
    size_t dest_size
) {
    size_t d = 0;
    dest[0] = '\0';

    for (
        size_t i = 0; src[i] != '\0' &&
        d < dest_size - 1;
    ) {
        if (src[i] == '$') {
            i++;
            int has_brace = 0;
            if (src[i] == '{') {
                has_brace = 1;
                i++;
            }

            char varname[256];
            size_t vn = 0;
            while (
                src[i] != '\0' && (
                    (
                        src[i] >= 'A' &&
                        src[i] <= 'Z'
                    ) ||
                    (
                        src[i] >= 'a' &&
                        src[i] <= 'z'
                    ) ||
                    (
                        src[i] >= '0' &&
                        src[i] <= '9'
                    ) ||
                    src[i] == '_'
                )
            ) {
                if (vn < sizeof(varname) - 1) {
                    varname[vn++] = src[i];
                }
                i++;
            }
            varname[vn] = '\0';

            if (has_brace && src[i] == '}') {
                i++;
            }

            const char *val = NULL;
            if (strcmp(varname, "PREFIX") == 0) {
                val = get_string_value("PREFIX");
                if (!val || !*val) {
                    val = "/usr";
                }
            } else {
                val = get_string_value(varname);
            }

            if (val) {
                size_t vlen = strlen(val);
                if (d + vlen < dest_size - 1) {
                    strcpy(dest + d, val);
                    d += vlen;
                }
            }
        } else {
            dest[d++] = src[i++];
        }
    }
    dest[d] = '\0';
}

static void traverse_and_find_dirs(
    const char *dirpath,
    StringBuffer *sb
) {
    if (!dirpath || !*dirpath) return;

    append_path(sb, dirpath);

    DIR *dir = opendir(dirpath);
    if (!dir) {
        return;
    }

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
    int is_readonly = 0;

    char *aresroot = get_string_value("__aresroot__");
    if (aresroot && *aresroot) {
        char config_file[PATH_MAX];
        snprintf(
            config_file,
            sizeof(config_file),
            "%s/config/startup.conf",
            aresroot
        );

        FILE *fp = fopen(config_file, "r");
        if (fp) {
            char line[4096];
            while (fgets(line, sizeof(line), fp)) {
                char *trimmed = trim_whitespace(line);
                if (strncmp(trimmed, "toolkit", 7) == 0) {
                    char *eq = strchr(trimmed, '=');
                    if (eq) {
                        char *val = eq + 1;

                        char *ro_pos = strstr(
                            val,
                            "<+readonly>"
                        );

                        if (ro_pos) {
                            is_readonly = 1;
                            *ro_pos = '\0';
                        } else {
                            char *nro_pos = strstr(
                                val,
                                "<-readonly>"
                            );

                            if (nro_pos) {
                                is_readonly = 0;
                                *nro_pos = '\0';
                            }
                        }

                        char *token = strtok(val, ",");
                        while (token) {
                            char *clean_tok = trim_whitespace(token);
                            if (*clean_tok) {
                                char expanded[PATH_MAX];
                                expand_vars(
                                    clean_tok,
                                    expanded,
                                    sizeof(expanded)
                                );

                                if (*expanded) {
                                    traverse_and_find_dirs(
                                        expanded,
                                        &new_path
                                    );
                                }
                            }
                            token = strtok(NULL, ",");
                        }
                        break;
                    }
                }
            }
            fclose(fp);
        }
    }

    char *curr_path = get_string_value("PATH");
    if (
        curr_path &&
        *curr_path
    ) {
        append_path_list(
            &new_path,
            curr_path
        );
    }

    if (new_path.str) {
        SHELL_VAR *old_v = find_variable("PATH");
        if (old_v) {
            VUNSETATTR(
                old_v,
                att_readonly
            );
        }

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

            if (is_readonly) {
                VSETATTR(
                    v,
                    att_readonly
                );
            }
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