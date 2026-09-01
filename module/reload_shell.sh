# https://github.com/Zeronetsec/Ares

function module::ReloadShell() {
    exec -a 'Ares Framework Reload' \
        ares --shell
}; readonly -f module::ReloadShell

# Copyright (c) 2026 Zeronetsec