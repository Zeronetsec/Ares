# https://github.com/Zeronetsec/Ares

function __fzf_dir_searcher__() {
    local selected_dir
    selected_dir="$(
        command find . -maxdepth 3 -type d \
            2>/dev/null | \
                command fzf \
                    --height 40% \
                    --layout=reverse \
                    --border \
                    --header="jump to?"
    )"
    if [[ -n "${selected_dir}" ]]; then
        cd "${selected_dir}" || return
        READLINE_LINE=""
        READLINE_POINT=0
    fi
}; readonly -f __fzf_dir_searcher__

# Copyright (c) 2026 Zeronetsec