# https://github.com/Zeronetsec/Ares

function utils::birthday() {
    local birth_date="08-23"
    local today="$(command date '+%m-%d')"
    if [[ "${today}" == "${birth_date}" ]]; then
        echo -e "${R}› ${N}Happy birthday for ${GG}Ares ${N}🎉"
        printf '\n'
    fi
}; readonly -f utils::birthday

# Copyright (c) 2026 Zeronetsec