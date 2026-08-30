# https://github.com/Zeronetsec/Ares

function module::Help() {
    local engine="${__aresroot__}/utils/go/json_parser"
    if [[ -f "${engine}" ]]; then
        utils::banner
        utils::birthday

        echo -e "${color_N}Usage: ${color_GG}ares ${color_CC}<option> [<args>]${color_N}"
        printf '\n'
        echo -e "${color_N}Available options:"

        [[ ! -x "${engine}" ]] && {
            command chmod +x "${engine}"
        }

        "${engine}"
        return ${?}
    else
        echo -e "${color_R}[!] ${color_N}Engine: ${color_GG}${engine} ${color_N}not found!"
        return 1
    fi

    return 0
}; readonly -f module::Help

# Copyright (c) 2026 Zeronetsec