// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "bufio"
    "strings"
)

func readCfg(path string) ([]ConfigItem, error) {
    file, err := os.Open(path)
    if err != nil {
        return nil, err
    }
    defer file.Close()

    var items []ConfigItem
    scanner := bufio.NewScanner(file)
    for scanner.Scan() {
        line := scanner.Text()
        trimmed := strings.TrimSpace(line)

        if trimmed == "" || strings.HasPrefix(trimmed, "#") {
            items = append(items, ConfigItem{
                IsComment: true,
                RawLine: line,
            })
            continue
        }

        parts := strings.SplitN(line, "=", 2)
        if len(parts) == 2 {
            items = append(items, ConfigItem{
                Key: strings.TrimSpace(parts[0]),
                Value: strings.TrimSpace(parts[1]),
                IsComment: false,
            })
        } else {
            items = append(items, ConfigItem{
                IsComment: true,
                RawLine: line,
            })
        }
    }
    return items, scanner.Err()
}

// Copyright (c) 2026 Zeronetsec