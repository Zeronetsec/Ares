# https://github.com/Zeronetsec/Ares

function __fzf_history_searcher__() {
    local selected_command="$(
        history | \
            command fzf \
                --tac \
                --no-sort \
                --exact \
                --query="${READLINE_LINE}" \
                --height 40% \
                --layout=reverse \
                --border
    )"
    if [[ -n "${selected_command}" ]]; then
        READLINE_LINE="$(
            echo -e "${selected_command}" | \
                command sed 's/^[ ]*[0-9]*[ ]*//'
        )"
        READLINE_POINT="${#READLINE_LINE}"
    fi
}; readonly -f __fzf_history_searcher__

# Copyright (c) 2026 Zeronetsec