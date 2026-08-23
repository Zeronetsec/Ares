function install::extern::utilsGoCompile() {
    local base_dir="${opt}/${targetins}/utils/go"

    local ugo
    for ugo in "${base_dir}"/*; do
        [[ -d "${ugo}" ]] || continue

        local utils_name="$(
            command basename "${ugo}"
        )"

        echo -e "${B}[*] ${N}Compile: ${GG}${utils_name} ${DG}(${WW}lang: ${GG}go ${DG}| ${WW}type: ${YY}utils${DG})${N}"

        local temp_bin="${base_dir}/.${utils_name}.tmp"
        local final_bin="${base_dir}/${utils_name}"

        (
            cd "${ugo}"
            command go mod tidy
            command go build -o "${temp_bin}" .
        )

        if [[ -f "${temp_bin}" ]]; then
            command rm -rf "${ugo}"
            command mv \
                "${temp_bin}" \
                "${final_bin}"
        fi
    done
}; readonly -f install::extern::utilsGoCompile