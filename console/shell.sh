# https://github.com/Zeronetsec/Ares

set -o errexit
readonly __aresroot__ || exit 1
readonly SHELL || exit 1

enable -f "${__aresroot__}/utils/libso/loadso.so" loadso || exit 1
builtin loadso : '(
    lib/shell/libso/error -> error
)' || exit 1

trap 'builtin error' ERR || exit 1

__aresloader__="${__init__}/loader.sh"
__aresdefrc__="${__console__}/ares.rc"

if [[ -f "${__aresloader__}" ]]; then
    source "${__aresloader__}"
else
    echo -e "${R}[!] ${N}Error: missing init loader in ${GG}${__aresloader__}${N}"
    printf '\033[?25h'
    exit 1
fi

builtin shmod

if [[ -f "${__aresrc__}" ]]; then
    afill="$(
        command grep -vE \
            '^\s*(#|$)' \
            "${__aresrc__}" \
            2>/dev/null || \
            true
    )"

    if [[ -n "${afill}" ]]; then
        source "${__aresrc__}"
    else
        source "${__aresdefrc__}"
    fi
else
    source "${__aresdefrc__}"
fi

builtin destroyv : '(
    src
    dir
    __aresloader__
    __aresdefrc__
    afill
)'

builtin llsi

builtin destroyso : '(
    shmod
    destroyv
    error
    loadso
    llsi
)'

enable -d destroyso

set +o errexit
trap - ERR

# Copyright (c) 2026 Zeronetsec