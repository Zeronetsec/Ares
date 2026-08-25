# https://github.com/Zeronetsec/Ares

function module::Exec() {
    if [[ -z "${1}" ]]; then
        utils::missingArgument
        return 1
    fi

    export SHELL="${0}"; readonly SHELL
    exec -a 'Ares Framework Console' \
        bash --rcfile \
        "${__aresroot__}/console/shell.sh" -i -c "${@}"
}; readonly -f module::Exec

# Copyright (c) 2026 Zeronetsec