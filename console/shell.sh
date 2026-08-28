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

set +o errexit
trap - ERR

# Copyright (c) 2026 Zeronetsec