# https://github.com/Zeronetsec/Ares

enable -f "${__aresroot__}/utils/libso/loadso.so" loadso
builtin loadso : '(
    utils/libso/include -> include
    utils/libso/destroyf -> destroyf
    utils/libso/destroyv -> destroyv
    utils/libso/destroyso -> destroyso
    utils/libso/rhome -> rhome
    utils/libso/dynav -> dynav
    utils/libso/dynap -> dynap
    utils/libso/shmod -> shmod
    utils/libso/unreadonlyf -> unreadonlyf
    utils/libso/catchypr -> catchypr
)'

builtin include : '(
    utils/variable
    utils/color
)'

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

: '<- console/shell.sh'

# Copyright (c) 2026 Zeronetsec