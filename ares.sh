#!/usr/bin/env bash
# https://github.com/Zeronetsec/Ares

set -o errexit

src="${BASH_SOURCE[0]}"
while [[ -h "${src}" ]]; do
    dir="$(
        cd -P "$(
            command dirname "${src}"
        )" > /dev/null 2>&1 && pwd
    )"
    src="$(command readlink "${src}")"
    [[ "${src}" != /* ]] && src="${dir}/${src}"
done

dir="$(
    cd -P "$(
        command dirname "${src}"
    )" > /dev/null 2>&1 && pwd
)"

export __aresroot__="${dir}"; readonly __aresroot__
enable -f "${__aresroot__}/utils/libso/loadso.so" loadso
builtin loadso : '(
    utils/libso/include -> include
    utils/libso/rhome -> rhome
    utils/libso/dynav -> dynav
    utils/libso/dynap -> dynap
)'

builtin include : '(
    utils/variable
    utils/color
    utils/missing_argument
    utils/invalid_option
    utils/banner
    utils/birthday
    module/shell
    module/exec
    module/help
    module/version
    module/uwu
    module/chstartup
    module/reload_shell
    module/show_toolkit
)'

case "${1}" in
    "")
        utils::missingArgument
        ;;
    "--shell")
        module::Shell
        ;;
    "--exec")
        module::Exec "${@:2}"
        ;;
    "--help")
        module::Help
        ;;
    "--version")
        module::Version
        ;;
    "--uwu")
        module::Uwu
        ;;
    "--chstartup")
        module::Chstartup
        ;;
    "--reload-shell")
        module::ReloadShell
        ;;
    "--show-toolkit")
        module::ShowToolkit "${@:2}"
        ;;
    *)
        utils::invalidOption "${1}"
        ;;
esac

# Copyright (c) 2026 Zeronetsec