// https://github.com/Zeronetsec/Ares

package shell

import (
    "os"
    "fmt"
    "strings"
    "os/exec"
)

func Execute(name string, args ...string) {
    fullCommand := name
    if len(args) > 0 {
        fullCommand = fmt.Sprintf(
            "%s %s", name, strings.Join(args, " "),
        )
    }

    cmd := exec.Command("bash", "-c", fullCommand)
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr
    cmd.Stdin = os.Stdin
    _ = cmd.Run()
}

// Copyright (c) 2026 Zeronetsec