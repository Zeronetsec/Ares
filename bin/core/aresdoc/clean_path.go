// https://github.com/Zeronetsec/Ares

package main

import (
    "strings"
    "os"
    "fmt"
    "path/filepath"
)

func cleanPath(pathStr string) string {
    aresRoot := strings.TrimRight(
        os.Getenv("__aresroot__"), "/",
    )

    sysPrefix := strings.TrimRight(
        os.Getenv("PREFIX"), "/",
    )

    normPath := filepath.Clean(pathStr)
    if aresRoot != "" && strings.HasPrefix(
        normPath, aresRoot,
    ) {
        rel, err := filepath.Rel(aresRoot, normPath)
        if err == nil {
            if rel == "." {
                return "ares"
            }
            return fmt.Sprintf(
                "ares/%s",
                rel,
            )
        }
    }

    if sysPrefix != "" && strings.HasPrefix(
        normPath, sysPrefix,
    ) {
        parent := filepath.Dir(sysPrefix)
        rel, err := filepath.Rel(parent, normPath)
        if err == nil {
            return rel
        }
    }

    return strings.TrimLeft(
        normPath, "/",
    )
}

// Copyright (c) 2026 Zeronetsec