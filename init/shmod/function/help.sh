# https://github.com/Zeronetsec/Ares

function help() {
    if [[ "${1}" == '--bash' ]]; then
        command bash -c 'help'
        return
    fi
    command ares --help
}; readonly -f help

# Copyright (c) 2026 Zeronetsec