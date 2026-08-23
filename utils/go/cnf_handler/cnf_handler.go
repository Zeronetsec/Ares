// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "os"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func main() {
    if len(os.Args) < 2 {
        os.Exit(1)
    }

    input := os.Args[1]
    fmt.Printf(
        "%sAres Shell: command %s%s %snot found!\n",
        color.N, color.GG, input, color.N,
    )

    commands := getCommands()
    if len(commands) == 0 {
        os.Exit(1)
    }

    suggests := findSuggestions(input, commands)
    if len(suggests) == 0 {
        os.Exit(1)
    }

    if len(suggests) == 1 ||
        (len(suggests) > 1 &&
            suggests[0].Score-suggests[1].Score > 0.20) {
                fmt.Printf(
                    "%s- %sDid you mean: %s%s%s?\n",
                    color.R, color.N, color.GG,
                    suggests[0].Command, color.N,
                )
                os.Exit(1)
            }

    fmt.Printf(
        "%s- %sDid you mean:\n",
        color.R, color.N,
    )

    limit := 3
    if len(suggests) < limit {
        limit = len(suggests)
    }

    for i := 0; i < limit; i++ {
        fmt.Printf(
            "    %s- %s%s%s\n",
            color.R, color.GG, suggests[i].Command, color.N,
        )
    }

    os.Exit(1)
}

// Copyright (c) 2026 Zeronetsec