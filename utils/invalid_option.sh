# https://github.com/Zeronetsec/Ares

function utils::invalidOption() {
    local input="${1}"
    echo -e "${color_R}[!] ${color_N}Invalid option: ${color_GG}${input}${color_N}"
    echo -e "${color_R}[!] ${color_N}Try: ${color_GG}ares --help${color_N}"
    return 1
}; readonly -f utils::invalidOption

# Copyright (c) 2026 Zeronetsec