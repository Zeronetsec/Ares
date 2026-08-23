function install::extern::libsoCCompile() {
    if [[ ! -d "${opt}/${targetins}/lib/shell/libso" ]]; then
        install::getinstall \
            "
                command mkdir -p \
                    ${opt}/${targetins}/lib/shell/libso
            " \
            "Create directory: ${GG}${opt}/${targetins}/lib/shell/libso${N}"
    fi

    local clibs
    command mapfile -t clibs < <(
        command ls \
            "${opt}/${targetins}/utils/libso/" \
            --color=never \
            2>/dev/null
    )

    local lib
    for lib in "${clibs[@]}"; do
        local libname="${lib%%.*}"
        ext="${lib##*.}"

        [[ "${ext,,}" != 'c' ]] && continue

        echo -e "${B}[*] ${N}Compile: ${GG}${libname}.so ${DG}(${WW}lang: ${GG}c ${DG}| ${WW}type: ${BB}libso${DG})${N}"
        #install::getinstall \
        #    "
                command gcc \
                    -fPIC -shared -O3 -march=native \
                    -s ${opt}/${targetins}/utils/libso/${libname}.c \
                    -o ${opt}/${targetins}/utils/libso/${libname}.so \
                    -I${prefix}/include/bash \
                    -I${prefix}/include/bash/include \
                    -I${prefix}/include/bash/builtins \
                    -I${opt}/${targetins}/lib/c
        #    " \
        #    "Compiling: ${GG}${libname}.so${N}"

        command ln -sf \
            ${opt}/${targetins}/utils/libso/${libname}.so \
            ${opt}/${targetins}/lib/shell/libso/${libname}.so

        command rm -rf \
            "${opt}/${targetins}/utils/libso/${libname}.c"
    done
}; readonly -f install::extern::libsoCCompile