# https://github.com/Zeronetsec/Ares

function lib::shell::invalidOption() {
    echo -e "${color_R}[!] ${color_N}Invalid option: ${color_GG}${1}${color_N}"
    echo -e "${color_R}[!] ${color_N}Try: ${color_GG}aresdoc ${color_CC}${2}${color_N}"
}; readonly -f lib::shell::invalidOption

# Copyright (c) 2026 Zeronetsec