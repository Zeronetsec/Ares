# https://github.com/Zeronetsec/Ares

function utils::invalidOption() {
    local input="${1}"
    echo -e "${R}[!] ${N}Invalid option: ${GG}${input}${N}"
    echo -e "${R}[!] ${N}Try: ${GG}ares --help${N}"
    return 1
}; readonly -f utils::invalidOption

# Copyright (c) 2026 Zeronetsec