# https://github.com/Zeronetsec/Ares

function lib::shell::invalidOption() {
    echo -e "${R}[!] ${N}Invalid option: ${GG}${1}${N}"
    echo -e "${R}[!] ${N}Try: ${GG}aresdoc ${CC}${2}${N}"
}; readonly -f lib::shell::invalidOption

# Copyright (c) 2026 Zeronetsec