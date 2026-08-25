#!/usr/bin/env bash

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

export root="${dir}"; readonly root
source "${root}/.install/include.sh"

include : '(
    .install/color
    .install/variable
    .install/checker
    .install/error
    .install/getinstall
    .install/inpackages
    .install/installer
    .install/prepdir
    .install/zinstall
    .install/zparser
    .install/chmod
    .install/backup
    .install/postins
    .install/symlink
    .install/extern/color
    .install/extern/libso_c_compile
    .install/extern/utils_go_compile
    .install/extern/bin_c_compile
    .install/extern/bin_go_compile
    .install/extern/remove_ext
)'

HOME="${HOME}"
__BACKUP__=false

while [[ ${#} -gt 0 ]]; do
    case "${1}" in
        "--home="*) export HOME="${1#*=}" ;;
        "--backup") export __BACKUP__=true ;;
    esac
    shift
done

install::inpackages
install::prepdir
install::backup
install::postins
install::installer
install::symlink
install::chmod
install::checker

trap - EXIT
exit ${?}