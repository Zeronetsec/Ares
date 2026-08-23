function install::extern::removeExt() {
    local target_dir="${opt}/${targetins}/bin"

    shopt -s globstar nullglob
    for file in "${target_dir}"/**; do
        [[ -f "${file}" ]] || continue

        local dir_path="${file%/*}"
        local filename="${file##*/}"

        [[ "${filename}" == *.* && "${filename}" != .* ]] || continue

        local base_name="${filename%.*}"
        local new_file="${dir_path}/${base_name}"

        if [[ ! -e "${new_file}" ]]; then
            command mv "${file}" "${new_file}"
            command chmod +x "${new_file}"
            continue
        fi

        local counter=2
        while [[ -e "${new_file}_v${counter}" ]]; do
            ((counter++))
        done

        command mv "${file}" "${new_file}_v${counter}"
        command chmod +x "${new_file}_v${counter}"
    done

    shopt -u globstar nullglob
}; readonly -f install::extern::removeExt