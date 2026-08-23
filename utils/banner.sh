# https://github.com/Zeronetsec/Ares

function utils::banner() {
    local banner="${__aresroot__}/data/banner.txt"
    echo -ne "${B}"
    command cat "${banner}"
    echo -ne "${N}"
    printf '\n'
}; readonly -f utils::banner

# Copyright (c) 2026 Zeronetsec