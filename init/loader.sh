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
    lib/shell/libso/llsi -> llsi
    lib/shell/libso/loadrc -> loadrc
)'

builtin include : '(
    utils/variable
    utils/color
)'

stconf="${__config__}/startup.conf"
gscache="${PREFIX}/goscript_cache"

if [[
    "$(
        command getconf "${stconf}" \
            --key 'remove_binext' \
            --get 1
    )" == true
]]; then
    command rmext "${__bin__}"
fi

if [[
    "$(
        command getconf "${stconf}" \
            --key 'chmod' \
            --get 1
    )" == true
]]; then
    command fchmod "${__bin__}" \
        --mode f0755 \
        --only file \
        > /dev/null 2>&1
fi

if [[
    "$(
        command getconf "${stconf}" \
            --key 'remove_log' \
            --get 1
    )" == true
]]; then
    command remake "${HOME}/.ares_log" \
        --mode 0644 \
        > /dev/null 2>&1
fi

if [[
    "$(
        command getconf "${stconf}" \
            --key 'remove_gscache' \
            --get 1
    )" == true
]]; then
    command remake "${gscache}" \
        --mode 0644 \
        > /dev/null 2>&1
fi

builtin llsi
builtin shmod
builtin loadrc

builtin destroyv : '(
    src
    dir
    __aresloader__
    __aresdefrc__
    stconf
    gscache
)'

builtin destroyso : '(
    include
    destroyf
    rhome
    dynav
    dynap
    unreadonlyf
    unreadonlyv
    shmod
    destroyv
    error
    loadso
    llsi
    loadrc
)'

enable -d destroyso

: '<- console/shell.sh'

# Copyright (c) 2026 Zeronetsec