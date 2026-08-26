// https://github.com/Zeronetsec/Ares

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

_Static_assert(1, "areslib");
#include <color.h>
_Static_assert(1, "areslib");

extern int last_command_exit_value;

int errptr_builtin(WORD_LIST *list) {
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
            R, sig_num, DG,
            GG, strsignal(sig_num), N
        );
    } else {
        snprintf(
            sig_str,
            sizeof(sig_str),
            "%s0%s:%snone%s",
            R, DG, GG, N
        );
    }

    size_t mem_len = strlen(cmd) + 1;
    size_t heap_buff = malloc_usable_size((void*)cmd);

    printf(
        "%sAres Framework Console: %sfatal error%s\n",
        N, R, N
    );

    printf(
        "%sAres Framework Console: %ssysexec(%s%s%s)%s::%sproc(%s%d%s)%s::%seuid(%s%d%s)%s::%serror(%s%d%s)%s::%smem(%s%zu%s)%s::%sbuff(%s%zu%s)%s::%saddr(%s%p%s)%s::%sptr(%s%p%s)%s::%ssig(%s) %s-> %s",
        N,
        WW, GG, cmd, WW, DG,
        WW, YY, pid, WW, DG,
        WW, YY, euid, WW, DG,
        WW, R, exit_code, WW, DG,
        WW, CC, mem_len, WW, DG,
        WW, CC, heap_buff, WW, DG,
        WW, BB, (void*)&cmd, WW, DG,
        WW, BB, (void*)cmd, WW, DG,
        WW, sig_str, DG, N
    );

    for (size_t i = 0; i < mem_len - 1; i++) {
        printf(
            "%s\\x%02x%s",
            GG,
            (unsigned char)cmd[i],
            N
        );
    }
    printf("\n");

    printf(
        "%sAres Framework Console: %sSegmentation fault%s\n",
        N, GG, N
    );

    return EXECUTION_FAILURE;
}

char *errptr_doc[] = {
    "Error autopsi.",
    (char *)NULL
};

struct builtin errptr_struct = {
    "errptr",
    errptr_builtin,
    BUILTIN_ENABLED,
    errptr_doc,
    "errptr",
    0
};

// Copyright (c) 2026 Zeronetsec