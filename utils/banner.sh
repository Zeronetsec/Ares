# https://github.com/Zeronetsec/Ares

function utils::banner() {
    local banner="${__aresroot__}/data/banner.txt"
    echo -ne "${color_B}"
    command cat "${banner}"
    echo -ne "${color_N}"
    printf '\n'
}; readonly -f utils::banner

# Copyright (c) 2026 Zeronetsec