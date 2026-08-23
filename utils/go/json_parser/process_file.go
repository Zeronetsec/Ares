// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "fmt"
    "encoding/json"
)

func processFile(filePath string) {
    fileData, err := os.ReadFile(filePath)
    if err != nil {
        return
    }

    var data Metadata
    if err := json.Unmarshal(
        fileData,
        &data,
    ); err != nil {
        return
    }

    var fullcmd string
    if data.Args != "" {
        fullcmd = fmt.Sprintf(
            "%s%s %s%s%s",
            GG, data.Command, CC, data.Args, N,
        )
    } else {
        fullcmd = fmt.Sprintf(
            "%s%s%s", GG, data.Command, N,
        )
    }

    fmt.Printf(
        "    %s* %s\n",
        DG, fullcmd,
    )

    fmt.Printf(
        "    %s└── %s%s%s\n",
        DG, WW, data.Description, N,
    )
}

// Copyright (c) 2026 Zeronetsec