// https://github.com/Zeronetsec/Ares

package main

import (
    "github.com/gdamore/tcell/v2"
)

func drawString(
    s tcell.Screen, x, y int,
    style tcell.Style, str string,
) {
    for _, r := range str {
        s.SetContent(x, y, r, nil, style)
        x++
    }
}

// Copyright (c) 2026 Zeronetsec