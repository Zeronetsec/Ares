// https://github.com/Zeronetsec/Ares

package main

import (
    "sort"
)

func findSuggestions(input string, commands []string) []Match {
    var matches []Match
    for _, cmd := range commands {
        score := similarityScore(input, cmd)
        if score < 0.55 {
            continue
        }

        matches = append(matches, Match{
            Command: cmd,
            Score: score,
        })
    }

    sort.Slice(matches, func(i, j int) bool {
        return matches[i].Score > matches[j].Score
    })

    if len(matches) > 5 {
        matches = matches[:5]
    }

    return matches
}

// Copyright (c) 2026 Zeronetsec