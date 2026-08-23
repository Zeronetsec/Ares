// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "os"
)

func main() {
    if len(os.Args) < 2 {
        os.Exit(1)
    }

    input := os.Args[1]
    fmt.Printf(
        "%sAres Shell: command %s%s %snot found!\n",
        N, GG, input, N,
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
                    R, N, GG, suggests[0].Command, N,
                )
                os.Exit(1)
            }

    fmt.Printf(
        "%s- %sDid you mean:\n",
        R, N,
    )

    limit := 3
    if len(suggests) < limit {
        limit = len(suggests)
    }

    for i := 0; i < limit; i++ {
        fmt.Printf(
            "    %s- %s%s%s\n",
            R, GG, suggests[i].Command, N,
        )
    }

    os.Exit(1)
}

// Copyright (c) 2026 Zeronetsec