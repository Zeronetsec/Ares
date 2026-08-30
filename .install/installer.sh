function install::installer() {
    install::extern::libsoCCompile

    install::extern::utilsGoCompile

    install::extern::binCCompile
    install::extern::binGoCompile

    install::extern::removeExt

    if [[ ! -d "${HOME}/.ares_log" ]]; then
        install::getinstall \
            "command mkdir -p ${HOME}/.ares_log" \
            "Create directory: ${color_GG}${HOME}/.ares_log${color_N}"
    fi

    if [[ ! -f "${HOME}/.aresrc" ]]; then
        install::getinstall \
            "command touch ${HOME}/.aresrc" \
            "Create file: ${color_GG}${HOME}/.aresrc${color_N}"
    fi

    if [[ ! -d "${HOME}/.ares/init/shmod" ]]; then
        install::getinstall \
            "command mkdir -p ${HOME}/.ares/init/shmod" \
            "Create directory: ${color_GG}${HOME}/.ares/init/shmod${color_N}"
    fi

    if [[ ! -d "${tmp}/goscript_cache" ]]; then
        install::getinstall \
            "command mkdir -p ${tmp}/goscript_cache" \
            "Create directory: ${color_GG}${tmp}/goscript_cache${color_N}"
    fi
}; readonly -f install::installer