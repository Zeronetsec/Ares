#!/usr/bin/env bash
# https://github.com/Zeronetsec/Ares

set -o errexit

enable -f "${__lib__}/shell/libso/include.so" include
builtin include : '(
    lib/shell/missing_argument
)'

TARGET_DIR="${__data__}/aresdoc"
if [[ ${#} -eq 0 ]]; then
    lib::shell::missingArgument "aresdoc"
    exit 1
fi

NAME_INPUT=""
USE_RAW=false

for arg in "${@}"; do
    case "${arg}" in
        "--raw")
            USE_RAW=true
            ;;
        *)
            if [[ -z "${NAME_INPUT}" ]]; then
                NAME_INPUT="${arg}"
            fi
            ;;
    esac
done

if [[ -z "${NAME_INPUT}" ]]; then
    lib::shell::missingArgument "aresdoc"
    exit 1
fi

if [[ ! -d "${TARGET_DIR}" ]]; then
    echo -e "${R}[!] ${N}Directory: ${GG}${TARGET_DIR} ${N}not found!"
    exit 1
fi

CLEAN_INPUT="$(
    echo "${NAME_INPUT}" | \
        command sed 's/::/\//g' | \
        command sed 's/^\///;s/\/$//'
)"

DIR_PART="$(command dirname "${CLEAN_INPUT}")"
BASE_PART="$(command basename "${CLEAN_INPUT}")"

if [[ "${DIR_PART}" == "." ]]; then
    DIR_PART=""
else
    DIR_PART="${DIR_PART}/"
fi

SNAKE_BASE="$(
    echo "${BASE_PART}" | \
        command sed -E 's/([a-z0-9])([A-Z])/\1_\2/g' | \
        command tr '[:upper:]' '[:lower:]'
)"

KEBAB_BASE="$(
    echo "${BASE_PART}" | \
        command sed -E 's/([a-z0-9])([A-Z])/\1-\2/g' | \
        command tr '[:upper:]' '[:lower:]'
)"

RAW_LOWER_BASE="$(
    echo "${BASE_PART}" | \
        command tr '[:upper:]' '[:lower:]'
)"

FOUND_FILES="$(
    command find -L "${TARGET_DIR}/${DIR_PART}" -type f \( \
        -iname "${RAW_LOWER_BASE}.md" -o \
        -iname "${SNAKE_BASE}.md" -o \
        -iname "${KEBAB_BASE}.md" \
    \) 2>/dev/null || true
)"

FOUND_FILES="$(
    echo "${FOUND_FILES}" | \
        command sed '/^$/d' | \
        command sort -u
)"

if [[ -z "${FOUND_FILES}" ]]; then
    echo -e "${R}[!] ${N}Documentation for: ${GG}${NAME_INPUT} ${N}not found!"
    exit 1
fi

FILE_COUNT="$(
    echo "${FOUND_FILES}" | \
        command wc -l | \
        command tr -d ' '
)"

SELECTED_FILE=""
if [[ "${FILE_COUNT}" -eq 1 ]]; then
    SELECTED_FILE="${FOUND_FILES}"
else
    SELECTED_FILE="$(
        echo "${FOUND_FILES}" |
            command fzf \
                --height=40% \
                --reverse \
                --header="Available documentations for ${NAME_INPUT}:"
    )"
fi

if [[ -z "${SELECTED_FILE}" ]]; then
    echo -e "${R}[!] ${N}No document selected!"
    exit 1
fi

if [[ "${USE_RAW}" = true ]]; then
    command bat \
        --color=always \
        --decorations=never \
        --paging=never \
        --language=markdown \
        "${SELECTED_FILE}"
else
    command glow \
        "${SELECTED_FILE}" \
        -w 0
fi

# Copyright (c) 2026 Zeronetsec