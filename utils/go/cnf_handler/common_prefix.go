// https://github.com/Zeronetsec/Ares

package main

func commonPrefix(a, b string) int {
    limit := len(a)
    if len(b) < limit {
        limit = len(b)
    }

    count := 0
    for i := 0; i < limit; i++ {
        if a[i] != b[i] {
            break
        }
        count++
    }

    return count
}

// Copyright (c) 2026 Zeronetsec