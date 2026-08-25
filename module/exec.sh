# https://github.com/Zeronetsec/Ares

function module::Exec() {
    if [[ -z "${1}" ]]; then
        utils::missingArgument
        return 1
    fi

    exec -a 'Ares Shell' \
        bash --rcfile \
        "${__aresroot__}/console/shell.sh" -i -c "${@}"
}; readonly -f module::Exec

# Copyright (c) 2026 Zeronetsec