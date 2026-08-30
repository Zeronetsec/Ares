# https://github.com/Zeronetsec/Ares

function utils::missingArgument() {
    echo -e "${color_R}[!] ${color_N}Missing argument!"
    echo -e "${color_R}[!] ${color_N}Try: ${color_GG}ares --help${color_N}"
    return 1
}; readonly -f utils::missingArgument

# Copyright (c) 2026 Zeronetsec