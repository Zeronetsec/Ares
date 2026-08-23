// https://github.com/Zeronetsec/Ares

package main

import (
    "strings"
)

func damerauLevenshtein(a, b string) int {
    ar := []rune(strings.ToLower(a))
    br := []rune(strings.ToLower(b))

    da := make(map[rune]int)
    maxDist := len(ar) + len(br)

    d := make([][]int, len(ar)+2)
    for i := range d {
        d[i] = make([]int, len(br)+2)
    }

    d[0][0] = maxDist
    for i := 0; i <= len(ar); i++ {
        d[i+1][0] = maxDist
        d[i+1][1] = i
    }

    for j := 0; j <= len(br); j++ {
        d[0][j+1] = maxDist
        d[1][j+1] = j
    }

    for i := 1; i <= len(ar); i++ {
        db := 0
        for j := 1; j <= len(br); j++ {
            i1 := da[br[j-1]]
            j1 := db

            cost := 1
            if ar[i-1] == br[j-1] {
                cost = 0
                db = j
            }

            sub := d[i][j] + cost
            ins := d[i+1][j] + 1
            del := d[i][j+1] + 1

            trans := d[i1][j1] + (i-i1-1) + 1 + (j-j1-1)

            d[i+1][j+1] = min(
                sub, min(
                    ins, min(del, trans),
                ),
            )
        }
        da[ar[i-1]] = i
    }
    return d[len(ar)+1][len(br)+1]
}

// Copyright (c) 2026 Zeronetsec