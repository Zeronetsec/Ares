// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "fmt"
    "encoding/json"
    "github.com/Zeronetsec/Ares/lib/go/color"
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
            color.GG, data.Command, color.CC, data.Args, color.N,
        )
    } else {
        fullcmd = fmt.Sprintf(
            "%s%s%s",
            color.GG, data.Command, color.N,
        )
    }

    fmt.Printf(
        "    %s* %s\n",
        color.DG, fullcmd,
    )

    fmt.Printf(
        "    %s└── %s%s%s\n",
        color.DG, color.WW, data.Description, color.N,
    )
}

// Copyright (c) 2026 Zeronetsec