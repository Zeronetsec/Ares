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
        echo -e "${R}[!] ${N}Engine: ${GG}${engine} ${N}not found!"
        return 1
    fi

    return 0
}; readonly -f module::Chstartup

# Copyright (c) 2026 Zeronetsec