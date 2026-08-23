// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "bufio"
    "fmt"
)

func saveCfg(path string, items []ConfigItem) error {
    file, err := os.Create(path)
    if err != nil {
        return err
    }
    defer file.Close()

    writer := bufio.NewWriter(file)
    for _, item := range items {
        if item.IsComment {
            _, _ = writer.WriteString(item.RawLine + "\n")
        } else {
            _, _ = writer.WriteString(fmt.Sprintf(
                "%s = %s\n",
                item.Key, item.Value,
            ))
        }
    }
    return writer.Flush()
}

// Copyright (c) 2026 Zeronetsec