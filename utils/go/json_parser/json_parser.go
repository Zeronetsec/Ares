// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "os"
    "path/filepath"
)

func main() {
    targetMetadata := os.Getenv("__aresroot__")
    if targetMetadata == "" {
        fmt.Printf(
            "%s[!] %sVariable: %s__aresroot__ %snot found!\n",
            R, N, GG, N,
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