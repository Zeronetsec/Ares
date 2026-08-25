# https://github.com/Zeronetsec/Ares

function module::Uwu() {
    local engine="${__aresroot__}/utils/go/nyanners"
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
}; readonly -f module::Uwu

# Copyright (c) 2026 Zeronetsec