// https://github.com/Zeronetsec/Ares

package main

import (
    "strings"
)

func toKebab(name string) string {
    res := camelCaseRe.ReplaceAllString(
        name,
        "${1}-${2}",
    )
    return strings.ToLower(res)
}

// Copyright (c) 2026 Zeronetsec