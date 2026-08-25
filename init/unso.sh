# https://github.com/Zeronetsec/Ares

function init::unso() {
    enable -f \
        "${__lib__}/shell/libso/destroyso.so" \
        destroyso || \
        return 1

    builtin destroyso : '(
        destroyf
        destroyv
        include
        loadso
        rhome
        dynav
        dynap
        shmod
        catchypr
        unreadonlyf
        unreadonlyv
        errptr
    )' || return 1

    enable -d destroyso || return 1
}; readonly -f init::unso; export -f init::unso

# Copyright (c) 2026 Zeronetsec