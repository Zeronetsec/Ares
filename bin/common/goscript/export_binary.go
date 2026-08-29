// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "fmt"
    "strings"
    "encoding/base64"
    "crypto/rand"
    "path/filepath"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func exportBinary(cfg Config, binaryCache string) {
    var outName string

    if cfg.CompileOut != "" {
        outName = cfg.CompileOut
    } else {
        if cfg.IsCode {
            randBytes := make([]byte, 6)
            rand.Read(randBytes)
            randomBase64 := base64.RawURLEncoding.EncodeToString(randBytes)
            outName = "bin_" + randomBase64
        } else {
            outName = strings.TrimSuffix(
                filepath.Base(cfg.SourceFile),
                filepath.Ext(cfg.SourceFile),
            )
            if outName == filepath.Base(cfg.SourceFile) {
                outName += "_bin"
            }
        }
    }

    destPath := filepath.Join(".", outName)
    binData, err := os.ReadFile(binaryCache)
    if err != nil {
        fmt.Printf(
            "%s[!] %sFailed to read compiled binary: %s%v%s\n",
            color.R, color.N, color.GG, err, color.N,
        )
        os.Exit(1)
    }

    if err := os.WriteFile(
        destPath,
        binData,
        0755,
    ); err != nil {
        fmt.Printf(
            "%s[!] %sFailed to save binary: %s%v%s\n",
            color.R, color.N, color.GG, err, color.N,
        )
        os.Exit(1)
    }

    fmt.Printf(
        "%s[+] %sBinary compiled successfully %s-> %s%s%s\n",
        color.GG, color.N, color.DG, color.GG, outName, color.N,
    )
    os.Exit(0)
}

// Copyright (c) 2026 Zeronetsec