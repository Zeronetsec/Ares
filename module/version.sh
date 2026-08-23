# https://github.com/Zeronetsec/Chprompt

function module::Version() {
    local name="Ares"
    local version="v0.1"
    local creator="Zeronetsec"
    local homepage="https://github.com/Zeronetsec/Ares"

    echo -e "${N}Name: ${GG}${name}${N}"
    echo -e "${N}Version: ${GG}${version}${N}"
    echo -e "${N}Creator: ${GG}${creator}${N}"
    echo -e "${N}Homepage: ${GG}${homepage}${N}"

    return 0
}; readonly -f module::Version

# Copyright (c) 2026 Zeronetsec