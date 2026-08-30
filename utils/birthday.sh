# https://github.com/Zeronetsec/Ares

function utils::birthday() {
    local birth_date="08-23"
    local today="$(command date '+%m-%d')"
    if [[ "${today}" == "${birth_date}" ]]; then
        echo -e "${color_R}› ${color_N}Happy birthday for ${color_GG}Ares ${color_N}🎉"
        printf '\n'
    fi
}; readonly -f utils::birthday

# Copyright (c) 2026 Zeronetsec