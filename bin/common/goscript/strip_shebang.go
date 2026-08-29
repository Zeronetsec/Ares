// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "strings"
)

func stripShebang(src, dst string) error {
    data, err := os.ReadFile(src)
    if err != nil {
        return err
    }

    lines := strings.Split(string(data), "\n")
    if len(lines) > 0 && strings.HasPrefix(
        lines[0], "#!",
    ) {
        lines = lines[1:]
    }

    var result strings.Builder

    hasPackage := false
    for _, line := range lines {
        if strings.HasPrefix(
            strings.TrimSpace(line),
            "package ",
        ) {
            hasPackage = true
            break
        }
    }

    if !hasPackage {
        result.WriteString(
            "package main\n\n",
        )
    }

    for _, line := range lines {
        result.WriteString(line)
        result.WriteString("\n")
    }

    return os.WriteFile(
        dst,
        []byte(result.String()),
        0644,
    )
}

// Copyright (c) 2026 Zeronetsec