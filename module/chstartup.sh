# https://github.com/Zeronetsec/Ares

function module::Chstartup() {
    local engine="${__aresroot__}/bin/core/chstartup"
    if [[ -f "${engine}" ]]; then

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
}; readonly -f module::Chstartup

# Copyright (c) 2026 Zeronetsec