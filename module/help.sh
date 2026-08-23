# https://github.com/Zeronetsec/Ares

function module::Help() {
    local engine="${__aresroot__}/utils/go/json_parser"
    if [[ -f "${engine}" ]]; then
        utils::banner
        utils::birthday

        echo -e "${N}Usage: ${GG}ares ${CC}<option> [<args>]${N}"
        printf '\n'
        echo -e "${N}Available options:"

        [[ ! -x "${engine}" ]] && {
            command chmod +x "${engine}"
        }

        "${engine}"
        return ${?}
    else
        echo -e "${R}[!] ${N}Engine: ${GG}${engine} ${N}not found!"
        return 1
    fi

    return 0
}; readonly -f module::Help

# Copyright (c) 2026 Zeronetsec