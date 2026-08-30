function install::extern::libsoCCompile() {
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

        install::getinstall \
            "
                command gcc \
                    -fPIC -shared -O3 -march=native \
                    -s ${opt}/${targetins}/utils/libso/${libname}.c \
                    -o ${opt}/${targetins}/utils/libso/${libname}.so \
                    -I${prefix}/include/bash \
                    -I${prefix}/include/bash/include \
                    -I${prefix}/include/bash/builtins \
                    -I${opt}/${targetins}/lib/c
            " \
            "Compiling: ${color_GG}${libname}.so ${color_DG}(${color_WW}lang: ${color_GG}c${color_DG}, ${color_WW}type: ${color_BB}libso${color_DG})${color_N}"

        command ln -sf \
            "${opt}/${targetins}/utils/libso/${libname}.so" \
            "${opt}/${targetins}/lib/shell/libso/${libname}.so"

        command rm -rf \
            "${opt}/${targetins}/utils/libso/${libname}.c"
    done
}; readonly -f install::extern::libsoCCompile