// https://github.com/Zeronetsec/Ares

package main

import (
    "strings"
    "fmt"
    "bytes"
    "os"
    "os/exec"
)

func pickFile(files []string, nameInput string) string {
    if len(files) == 0 {
        return ""
    }

    if len(files) == 1 {
        return files[0]
    }

    var sb strings.Builder
    for _, f := range files {
        sb.WriteString(
            fmt.Sprintf(
                "%s\t%s\n",
                cleanPath(f), f,
            ),
        )
    }

    cmd := exec.Command(
        "fzf",
        "--height=40%",
        "--reverse",
        fmt.Sprintf(
            "--header=Available documentations for %s:",
            nameInput,
        ),
        "--delimiter=\t",
        "--with-nth=1",
    )

    cmd.Stdin = strings.NewReader(
        sb.String(),
    )

    var out bytes.Buffer
    cmd.Stdout = &out
    cmd.Stderr = os.Stderr

    err := cmd.Run()
    if err != nil {
        return ""
    }

    selected := strings.TrimSpace(out.String())
    if selected == "" {
        return ""
    }

    parts := strings.Split(
        selected, "\t",
    )

    return parts[len(parts)-1]
}

// Copyright (c) 2026 Zeronetsec