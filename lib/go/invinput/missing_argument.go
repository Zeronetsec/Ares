// https://github.com/Zeronetsec/Ares

package invinput

import (
    "fmt"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func MissingArgument(command string) {
    fmt.Printf(
        "%s[!] %sMissing argument!\n",
        color.R, color.N,
    )

    fmt.Printf(
        "%s[!] %sTry: %saresdoc %s%s%s\n",
        color.R, color.N, color.GG, color.CC, command, color.N,
    )
}

// Copyright (c) 2026 Zeronetsec