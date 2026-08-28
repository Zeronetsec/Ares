# https://github.com/Zeronetsec/Ares

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
    lib/shell/libso/error -> error
)'

trap 'builtin error' ERR

builtin include : '(
    utils/variable
    utils/color
)'

stconf="${__config__}/startup.conf"
if [[
    "$(
        command getconf "${stconf}" \
            --key 'chmod' \
            --get 1
    )" == true
]]; then
    command fchmod "${__bin__}" \
        --mode 0755 \
        --only file > /dev/null 2>&1
fi

if [[
    "$(
        command getconf "${stconf}" \
            --key 'remove_log' \
            --get 1
    )" == true
]]; then
    command rm -rf "${HOME}/.ares_log"
    command mkdir -p "${HOME}/.ares_log"
fi

builtin destroyv : '(
    stconf
)'

builtin destroyso : '(
    include
    destroyf
    rhome
    dynav
    dynap
    unreadonlyf
    unreadonlyv
)'

: '<- console/shell.sh'

# Copyright (c) 2026 Zeronetsec