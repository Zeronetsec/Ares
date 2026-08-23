// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "time"
)

func uwu() {
    faces := []string{
        "(｡◕‿◕｡)",
        "(≧◡≦)",
        "ʕ•ᴥ•ʔ",
        "(・ω・)",
        "(๑˃ᴗ˂)ﻭ",
        "(ง'̀-'́)ง",
        "(=^･ω･^=)",
    }

    delay := 200 * time.Millisecond
    duration := 5 * time.Second
    startTime := time.Now()

    fmt.Print("\x1b[?25l")
    defer fmt.Print("\x1b[?25h\n")

    for time.Since(startTime) < duration {
        for _, face := range faces {
            if time.Since(startTime) >= duration {
                break
            }
            fmt.Printf(
                "\r%s\x1b[K",
                face,
            )
            time.Sleep(delay)
        }
    }
}

// Copyright (c) 2026 Zeronetsec