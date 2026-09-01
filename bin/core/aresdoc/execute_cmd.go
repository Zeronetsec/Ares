// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "os/exec"
)

func executeCmd(name string, args ...string) {
    cmd := exec.Command(name, args...)
    cmd.Stdin = os.Stdin
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    cmd.Run()
}

// Copyright (c) 2026 Zeronetsec