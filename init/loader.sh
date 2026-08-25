# https://github.com/Zeronetsec/Ares

set -o errexit

readonly __aresroot__
enable -f "${__aresroot__}/utils/libso/loadso.so" loadso
builtin loadso : '(
    lib/shell/libso/destroyf -> destroyf
    lib/shell/libso/destroyso -> destroyso
    lib/shell/libso/destroyv -> destroyv
    lib/shell/libso/include -> include
    lib/shell/libso/loadso -> loadso
    lib/shell/libso/rhome -> rhome
    lib/shell/libso/dynav -> dynav
    lib/shell/libso/dynap -> dynap
    lib/shell/libso/shmod -> shmod
    lib/shell/libso/catchypr -> catchypr
    lib/shell/libso/unreadonlyf -> unreadonlyf
    lib/shell/libso/unreadonlyv -> unreadonlyv
)'

builtin include : '(
    utils/variable
    utils/color
)'

: 'hit cnf_handler'
command chhmi > /dev/null 2>&1 || true

stconf="${__config__}/startup.conf"
if [[ "$(command getconf "${stconf}" --key 'chmod' --get 1)" == true ]]; then
    command fchmod "${__bin__}" \
        --mode 0755 \
        --only file > /dev/null 2>&1
fi

if [[ "$(command getconf "${stconf}" --key 'remove_log' --get 1)" == true ]]; then
    command rm -rf "${HOME}/.ares_log"
    command mkdir -p "${HOME}/.ares_log"
fi

builtin destroyv : '(
    src
    dir
    stconf
)'

builtin destroyso : '(
    include
    destroyf
    rhome
    dynav
    dynap
    unreadonlyf
)'

set +o errexit

: '<- console/shell.sh'

# Copyright (c) 2026 Zeronetsec