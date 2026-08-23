// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "os"
    "path/filepath"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func main() {
    targetMetadata := os.Getenv("__aresroot__")
    if targetMetadata == "" {
        fmt.Printf(
            "%s[!] %sVariable: %s__aresroot__ %snot found!\n",
            color.R, color.N, color.GG, color.N,
        )
        os.Exit(1)
    }

    metadataPath := filepath.Join(
        targetMetadata,
        "metadata",
        "*.json",
    )

    files, err := filepath.Glob(metadataPath)
    if err != nil {
        return
    }

    for _, filePath := range files {
        processFile(filePath)
    }
}

// Copyright (c) 2026 Zeronetsec