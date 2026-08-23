function install::extern::binCCompile() {
    local target_dir="${opt}/${targetins}/bin"

    (
        cd "${target_dir}"
        command find . -type f -name "*.c" | \
            while read -r c_file; do
                local dir_path="$(
                    command dirname "${c_file}"
                )"

                local tool_name="$(
                    command basename "${dir_path}"
                )"

                local expected_c="${dir_path}/${tool_name}.c"

                if [[ "${c_file}" == "${expected_c}" ]]; then
                    echo -e "${B}[*] ${N}Compile: ${GG}${tool_name} ${DG}(${WW}lang: ${GG}c ${DG}| ${WW}type: ${color_CC}bin${DG})${N}"

                    local temp_bin="./.${tool_name}.tmp"
                    local final_bin="$(
                        command dirname "${dir_path}"
                    )/${tool_name}"

                    command gcc \
                        -O3 -march=native -flto \
                        -s "${c_file}" \
                        -o "${temp_bin}" \
                        -I"${opt}/${targetins}/lib/c"

                    if [[ -f "${temp_bin}" ]]; then
                        command rm -rf "${dir_path}"
                        command mv \
                            "${temp_bin}" \
                            "${final_bin}"
                    fi
                fi
            done
    )
}; readonly -f install::extern::binCCompile