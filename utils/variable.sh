# https://github.com/Zeronetsec/Ares

export PREFIX="${PREFIX:-/usr}"

builtin dynav
builtin rhome
builtin dynap

export __aresrc__="${HOME}/.aresrc"; readonly __aresrc__
export __areslog__="${HOME}/.ares_log"; readonly __areslog__
export __shmoduser__="${HOME}/.ares/init/shmod"; readonly __shmoduser__

# Copyright (c) 2026 Zeronetsec