// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "strings"
    "sort"
    "path/filepath"
    "io/fs"
)

func findDocs(targetDir, dirPart, basePart string) []string {
    searchRoot := filepath.Join(targetDir, dirPart)
    if _, err := os.Stat(searchRoot); os.IsNotExist(err) {
        return nil
    }

    names := map[string]bool{
        strings.ToLower(basePart): true,
        toSnake(basePart): true,
        toKebab(basePart): true,
    }

    candidates := make(map[string]bool)
    for n := range names {
        candidates[n+".md"] = true
        candidates[n+".acon"] = true
    }

    var found []string
    for c := range candidates {
        p := filepath.Join(searchRoot, c)
        if fi, err := os.Stat(p); err == nil && !fi.IsDir() {
            found = append(found, p)
        }
    }

    if len(found) > 0 {
        sort.Strings(found)
        return found
    }

    filepath.WalkDir(
        searchRoot,
        func(
            path string,
            d fs.DirEntry,
            err error,
        ) error {
            if err != nil {
                return nil
            }

            if d.IsDir() {
                name := d.Name()
                if strings.HasPrefix(name, ".") && name != "." {
                    return filepath.SkipDir
                }
                return nil
            }

            if candidates[strings.ToLower(d.Name())] {
                found = append(found, path)
            }
            return nil
        },
    )

    uniqueMap := make(map[string]bool)
    var result []string
    for _, f := range found {
        if !uniqueMap[f] {
            uniqueMap[f] = true
            result = append(result, f)
        }
    }

    sort.Strings(result)
    return result
}

// Copyright (c) 2026 Zeronetsec