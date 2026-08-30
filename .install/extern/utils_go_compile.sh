function install::extern::utilsGoCompile() {
    local base_dir="${opt}/${targetins}/utils/go"

    local ugo
    for ugo in "${base_dir}"/*; do
        [[ -d "${ugo}" ]] || continue

        local utils_name="$(
            command basename "${ugo}"
        )"

        local temp_bin="${base_dir}/.${utils_name}.tmp"
        local final_bin="${base_dir}/${utils_name}"

        (
            cd "${ugo}"
            install::getinstall \
                "
                    command go mod tidy
                    command go build -o ${temp_bin} .
                " \
                "Compiling: ${color_GG}${utils_name} ${color_DG}(${color_WW}lang: ${color_GG}go${color_DG}, ${color_WW}type: ${color_YY}utils${color_DG})${color_N}"
        )

        if [[ -f "${temp_bin}" ]]; then
            command rm -rf "${ugo}"
            command mv \
                "${temp_bin}" \
                "${final_bin}"
        fi
    done
}; readonly -f install::extern::utilsGoCompile