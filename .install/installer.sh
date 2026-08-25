function install::installer() {
    install::extern::libsoCCompile

    install::extern::utilsGoCompile

    install::extern::binCCompile
    install::extern::binGoCompile

    install::extern::removeExt

    if [[ ! -d "${HOME}/.ares_log" ]]; then
        install::getinstall \
            "command mkdir -p ${HOME}/.ares_log" \
            "Create directory: ${GG}${HOME}/.ares_log${N}"
    fi

    if [[ ! -f "${HOME}/.aresrc" ]]; then
        install::getinstall \
            "command touch ${HOME}/.aresrc" \
            "Create file: ${GG}${HOME}/.aresrc${N}"
    fi

    if [[ ! -d "${HOME}/.ares/init/shmod" ]]; then
        install::getinstall \
            "command mkdir -p ${HOME}/.ares/init/shmod" \
            "Create directory: ${GG}${HOME}/.ares/init/shmod${N}"
    fi
}; readonly -f install::installer