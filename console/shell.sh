# https://github.com/Zeronetsec/Ares

if [[ -f "${__aresloader__}" ]]; then
    source "${__aresloader__}"
else
    echo -e "\033[1;31m[!] \033[0mError: missing init loader in \033[0;32m${__aresloader__}\033[0m"
    printf '\033[?25h'
    exit 1
fi

builtin shmod

if [[ -f "${__aresrc__}" ]]; then
    afill="$(
        command grep -vE \
            '^\s*(#|$)' \
            "${__aresrc__}" \
            2>/dev/null
    )"

    if [[ -n "${afill}" ]]; then
        source "${__aresrc__}"
    else
        source "${__aresdefrc__}"
    fi
else
    source "${__aresdefrc__}"
fi

builtin destroyv : '( afill )'
builtin destroyso : '(
    shmod
    destroyv
)'

enable -d destroyso

# Copyright (c) 2026 Zeronetsec