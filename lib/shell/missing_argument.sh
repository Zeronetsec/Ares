# https://github.com/Zeronetsec/Ares

function lib::shell::missingArgument() {
    echo -e "${color_R}[!] ${color_N}Missing argument!"
    echo -e "${color_R}[!] ${color_N}Try: ${color_GG}aresdoc ${color_CC}${1}${color_N}"
}; readonly -f lib::shell::missingArgument

# Copyright (c) 2026 Zeronetsec