# https://github.com/Zeronetsec/Ares

function init::rexso() {
    enable -f \
        "${__lib__}/shell/libso/loadso.so" \
        loadso || \
        return 1

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
    )' || return 1
}; readonly -f init::rexso; export -f init::rexso

# Copyright (c) 2026 Zeronetsec