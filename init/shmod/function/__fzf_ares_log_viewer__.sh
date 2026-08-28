# https://github.com/Zeronetsec/Ares

function __fzf_ares_log_viewer__() {
    local log_dir="${__areslog__:-.}"
    local selected_log="$(
        command find "${log_dir}" -type f \( -name "*.log" -o -name "*.txt" \) \
            2>/dev/null | \
                command fzf \
                    --height 40% \
                    --layout=reverse \
                    --border \
                    --query="${READLINE_LINE}" \
                    --preview-window="up:60%:wrap:border-bottom" \
                    --preview '
                        command bat \
                            --color=always \
                            --paging=never \
                            --style=numbers \
                            --line-range -500: \
                            {} 2>/dev/null || \
                                command tail -n 500 {}
                    '
    )"
    if [[ -n "${selected_log}" ]]; then
        READLINE_LINE="command tail -f ${selected_log}"
        READLINE_POINT="${#READLINE_LINE}"
    fi
}; readonly -f __fzf_ares_log_viewer__

# Copyright (c) 2026 Zeronetsec