# https://github.com/Zeronetsec/Ares

function lib::shell::missingArgument() {
    echo -e "${R}[!] ${N}Missing argument!"
    echo -e "${R}[!] ${N}Try: ${GG}aresdoc ${CC}${1}${N}"
    return 1
}; readonly -f lib::shell::missingArgument

# Copyright (c) 2026 Zeronetsec