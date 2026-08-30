# https://github.com/Zeronetsec/Ares

function command_not_found_handle() {
    if [[ -f "${__utils__}/go/cnf_handler" ]]; then
        "${__utils__}/go/cnf_handler" "${1}"
        return ${?}
    fi
    echo -e "${color_N}Ares Framework Console: command ${color_GG}${1} ${color_N}not found!"
    return 1
}; readonly -f command_not_found_handle

# Copyright (c) 2026 Zeronetsec