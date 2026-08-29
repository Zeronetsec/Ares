// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
)

func main() {
    cfg := parseArgs(os.Args)
    binaryCache := buildScript(cfg)
    if cfg.IsCompile {
        exportBinary(cfg, binaryCache)
    } else {
        runBinary(cfg, binaryCache)
    }
}

// Copyright (c) 2026 Zeronetsec