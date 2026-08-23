// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "path/filepath"
)

func getCommands() []string {
    seen := make(map[string]struct{})
    for _, dir := range filepath.SplitList(os.Getenv("PATH")) {
        entries, err := os.ReadDir(dir)
        if err != nil {
            continue
        }

        for _, entry := range entries {
            fullPath := filepath.Join(dir, entry.Name())
            info, err := os.Stat(fullPath)
            if err != nil {
                continue
            }

            if !info.Mode().IsRegular() {
                continue
            }

            if info.Mode().Perm()&0111 == 0 {
                continue
            }

            seen[entry.Name()] = struct{}{}
        }
    }

    commands := make([]string, 0, len(seen))
    for cmd := range seen {
        commands = append(commands, cmd)
    }

    return commands
}

// Copyright (c) 2026 Zeronetsec