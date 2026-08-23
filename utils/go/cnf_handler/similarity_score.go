// https://github.com/Zeronetsec/Ares

package main

import (
    "strings"
)

func similarityScore(input, cmd string) float64 {
    input = strings.ToLower(input)
    cmd = strings.ToLower(cmd)

    dist := damerauLevenshtein(input, cmd)

    maxLen := len(input)
    if len(cmd) > maxLen {
        maxLen = len(cmd)
    }

    if maxLen == 0 {
        return 0
    }

    score := 1.0 - float64(dist)/float64(maxLen)
    score += float64(commonPrefix(input, cmd)) * 0.08
    score += positionMatch(input, cmd) * 0.35

    if strings.Contains(cmd, input) {
        score += 0.15
    }

    if strings.Contains(input, cmd) {
        score += 0.15
    }

    return score
}

// Copyright (c) 2026 Zeronetsec