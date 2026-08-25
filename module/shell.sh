# https://github.com/Zeronetsec/Ares

function module::Shell() {
    export SHELL="${0}"; readonly SHELL
    exec -a 'Ares Framework Console' \
        bash --rcfile \
        "${__aresroot__}/console/shell.sh" -i
}; readonly -f module::Shell

# Copyright (c) 2026 Zeronetsec