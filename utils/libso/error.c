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
#include <string.h>
#include <unistd.h>
#include <malloc.h>
#include <signal.h>
#include <builtins.h>
#include <shell.h>
#include <bashgetopt.h>
#include <variables.h>

extern int last_command_exit_value;

int error_builtin(WORD_LIST *list) {
    char *cmd = get_string_value("BASH_COMMAND");
    if (!cmd) {
        cmd = "null";
    }

    pid_t pid = getpid();
    uid_t euid = geteuid();

    int exit_code = last_command_exit_value;
    int sig_num = (
        exit_code > 128 &&
        exit_code <= 128 + 64
    ) ? (exit_code - 128) : 0;

    char sig_str[64];
    if (sig_num > 0) {
        snprintf(
            sig_str,
            sizeof(sig_str),
            "%s%d%s:%s%s%s",
            color_R, sig_num, color_DG,
            color_GG, strsignal(sig_num), color_N
        );
    } else {
        snprintf(
            sig_str,
            sizeof(sig_str),
            "%s0%s:%snone%s",
            color_R, color_DG, color_GG, color_N
        );
    }

    size_t mem_len = strlen(cmd) + 1;
    size_t heap_buff = malloc_usable_size((void*)cmd);

    printf(
        "%sAres Framework Console: %sfatal error%s\n",
        color_N, color_R, color_N
    );

    printf(
        "%sAres Framework Console: %ssysexec(%s%s%s)%s::%sproc(%s%d%s)%s::%seuid(%s%d%s)%s::%serror(%s%d%s)%s::%smem(%s%zu%s)%s::%sbuff(%s%zu%s)%s::%saddr(%s%p%s)%s::%sptr(%s%p%s)%s::%ssig(%s) %s-> %s",
        color_N,
        color_WW, color_GG, cmd, color_WW, color_DG,
        color_WW, color_YY, pid, color_WW, color_DG,
        color_WW, color_YY, euid, color_WW, color_DG,
        color_WW, color_R, exit_code, color_WW, color_DG,
        color_WW, color_CC, mem_len, color_WW, color_DG,
        color_WW, color_CC, heap_buff, color_WW, color_DG,
        color_WW, color_BB, (void*)&cmd, color_WW, color_DG,
        color_WW, color_BB, (void*)cmd, color_WW, color_DG,
        color_WW, sig_str, color_DG, color_N
    );

    for (size_t i = 0; i < mem_len - 1; i++) {
        printf(
            "%s\\x%02x%s",
            color_GG,
            (unsigned char)cmd[i],
            color_N
        );
    }
    printf("\n");

    printf(
        "%sAres Framework Console: %sSegmentation fault%s\n",
        color_N, color_GG, color_N
    );

    return EXECUTION_FAILURE;
}

char *error_doc[] = {
    "Error autopsi.",
    (char *)NULL
};

struct builtin error_struct = {
    "error",
    error_builtin,
    BUILTIN_ENABLED,
    error_doc,
    "error",
    0
};

// Copyright (c) 2026 Zeronetsec