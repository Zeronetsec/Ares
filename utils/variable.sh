# https://github.com/Zeronetsec/Ares

export PREFIX="${PREFIX:-/usr}"

builtin dynav
builtin rhome
builtin dynap

export __aresloader__="${__init__}/loader.sh"; readonly __aresloader__
export __aresdefrc__="${__console__}/ares.rc"; readonly __aresdefrc__
export __aresrc__="${HOME}/.aresrc"; readonly __aresrc__
export __areslog__="${HOME}/.ares_log"; readonly __areslog__

# Copyright (c) 2026 Zeronetsec