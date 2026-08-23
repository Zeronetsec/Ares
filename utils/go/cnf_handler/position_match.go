// https://github.com/Zeronetsec/Ares

package main

func positionMatch(a, b string) float64 {
    limit := len(a)
    if len(b) < limit {
        limit = len(b)
    }

    if limit == 0 {
        return 0
    }

    match := 0
    for i := 0; i < limit; i++ {
        if a[i] == b[i] {
            match++
        }
    }

    return float64(match)/float64(limit)
}

// Copyright (c) 2026 Zeronetsec