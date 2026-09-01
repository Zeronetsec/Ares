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
                    local temp_bin="./.${tool_name}.tmp"
                    local final_bin="$(
                        command dirname "${dir_path}"
                    )/${tool_name}"

                    install::getinstall \
                        "
                            command gcc \
                                -O3 -march=native -flto \
                                -s ${c_file} \
                                -o ${temp_bin} \
                                -I${opt}/${targetins}/lib/c \
                                -I${target_dir}/${dir_path}
                        " \
                        "Compiling: ${color_GG}${tool_name} ${color_DG}(${color_WW}lang: ${color_GG}c${color_DG}, ${color_WW}type: ${color_CC}bin${color_DG})${color_N}"

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