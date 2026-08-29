// https://github.com/Zeronetsec/Ares

package main

import (
    "syscall"
    "os"
    "fmt"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func runBinary(cfg Config, binaryCache string) {
    binaryArgs := append(
        []string{binaryCache},
        cfg.ExtraArgs...,
    )

    if err := syscall.Exec(
        binaryCache,
        binaryArgs,
        os.Environ(),
    ); err != nil {
        fmt.Printf(
            "%s[!] %sFailed to execute binary: %s%v%s\n",
            color.R, color.N, color.GG, err, color.N,
        )
        os.Exit(1)
    }
}

// Copyright (c) 2026 Zeronetsec