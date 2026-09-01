// https://github.com/Zeronetsec/Ares

package invalid_input

import (
    "fmt"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func Execute(args, command string) {
    fmt.Printf(
        "%s[!] %sInvalid option: %s%s%s\n",
        color.R, color.N, color.GG, args, color.N,
    )

    fmt.Printf(
        "%s[!] %sTry: %saresdoc %s%s%s\n",
        color.R, color.N, color.GG, color.CC, command, color.N,
    )
}

// Copyright (c) 2026 Zeronetsec