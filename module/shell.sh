# https://github.com/Zeronetsec/Ares

function module::Shell() {
    exec -a 'Ares Shell' \
        bash --rcfile \
        "${__aresroot__}/console/shell.sh" -i
}; readonly -f module::Shell

# Copyright (c) 2026 Zeronetsec