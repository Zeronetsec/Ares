function install::extern::binGoCompile() {
    local target_dir="${opt}/${targetins}/bin"

    (
        cd "${target_dir}" || return 1
        command find . -type f -name "*.go" | \
            while read -r go_file; do
                local dir_path="$(
                    command dirname "${go_file}"
                )"

                local tool_name="$(
                    command basename "${dir_path}"
                )"

                local expected_go="${dir_path}/${tool_name}.go"

                if [[ "${go_file}" == "${expected_go}" ]]; then
                    #echo -e "${B}[*] ${N}Compile: ${GG}${tool_name} ${DG}(${WW}lang: ${GG}go ${DG}| ${WW}type: ${color_CC}bin${DG})${N}"

                    local temp_bin="${target_dir}/.${tool_name}.tmp"
                    local final_bin="$(
                        command dirname "${dir_path}"
                    )/${tool_name}"

                    (
                        cd "${dir_path}"
                        install::getinstall \
                            "
                                command go mod tidy
                                command go build -o ${temp_bin}
                            " \
                            "Compiling: ${GG}${tool_name} ${DG}(${WW}lang: ${GG}go${DG}, ${WW}type: ${color_CC}bin${DG})${N}"
                    )

                    if [[ -f "${temp_bin}" ]]; then
                        command rm -rf "${dir_path}"
                        command mv \
                            "${temp_bin}" \
                            "${final_bin}"
                    fi
                fi
            done
    )
}; readonly -f install::extern::binGoCompile
