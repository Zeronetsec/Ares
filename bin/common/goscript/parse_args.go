// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "strings"
    "github.com/Zeronetsec/Ares/lib/go/missing_argument"
)

func parseArgs(args []string) Config {
    if len(args) < 2 {
        missing_argument.Execute("goscript")
        os.Exit(1)
    }

    var cfg Config
    var actualArgs []string

    for i := 1; i < len(args); i++ {
        if args[i] == "--compile" {
            cfg.IsCompile = true
            if i+1 < len(args) && !strings.HasPrefix(
                args[i+1], "-",
            ) {
                cfg.CompileOut = args[i+1]
                i++
            }
        } else {
            actualArgs = append(actualArgs, args[i])
        }
    }

    if len(actualArgs) == 0 {
        missing_argument.Execute("goscript")
        os.Exit(1)
    }

    if actualArgs[0] == "--code" {
        cfg.IsCode = true
        if len(actualArgs) < 2 {
            missing_argument.Execute("goscript")
            os.Exit(1)
        }
        cfg.CodeContent = actualArgs[1]
        cfg.ExtraArgs = actualArgs[2:]
    } else {
        cfg.SourceFile = actualArgs[0]
        cfg.ExtraArgs = actualArgs[1:]
    }

    return cfg
}

// Copyright (c) 2026 Zeronetsec