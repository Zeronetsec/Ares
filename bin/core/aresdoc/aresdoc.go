// https://github.com/Zeronetsec/Ares

package main

import(
    "os"
    "strings"
    "fmt"
    "path/filepath"
    "github.com/Zeronetsec/Ares/lib/go/color"
    "github.com/Zeronetsec/Ares/lib/go/invinput"
)

func main() {
    dataDir := os.Getenv("__data__")
    targetDir := filepath.Join(dataDir, "aresdoc")

    args := os.Args[1:]
    if len(args) == 0 {
        invinput.MissingArgument("aresdoc")
        os.Exit(1)
    }

    nameInput := ""
    useRaw := false

    for _, arg := range args {
        if arg == "--raw" {
            useRaw = true
        } else if nameInput == "" {
            nameInput = arg
        }
    }

    if nameInput == "" {
        invinput.MissingArgument("aresdoc")
        os.Exit(1)
    }

    if stat, err := os.Stat(
        targetDir,
    ); os.IsNotExist(err) || !stat.IsDir() {
        fmt.Printf(
            "%s[!] %sDirectory: %s%s %snot found!\n",
            color.R, color.N, color.GG, targetDir, color.N,
        )
        os.Exit(1)
    }

    cleanInput := strings.Trim(
        strings.ReplaceAll(
            nameInput, "::", "/",
        ), "/",
    )

    dirPart := filepath.Dir(cleanInput)
    if dirPart == "." {
        dirPart = ""
    } else {
        dirPart = dirPart + "/"
    }
    basePart := filepath.Base(cleanInput)

    foundFiles := findDocs(
        targetDir,
        dirPart,
        basePart,
    )

    if len(foundFiles) == 0 {
        fmt.Printf(
            "%s[!] %sDocumentation for: %s%s %snot found!\n",
            color.R, color.N, color.GG, nameInput, color.N,
        )
        os.Exit(1)
    }

    selectedFile := pickFile(foundFiles, nameInput)
    if selectedFile == "" {
        fmt.Printf(
            "%s[!] %sNo document selected!\n",
            color.R, color.N,
        )
        os.Exit(1)
    }

    if useRaw {
        executeCmd(
            "bat",
            "--color=always",
            "--decorations=never",
            "--paging=never",
            selectedFile,
        )

        fmt.Printf(
            "%s\n",
            color.N,
        )
    } else {
        ext := strings.ToLower(
            filepath.Ext(selectedFile),
        )

        if ext == ".acon" {
            executeCmd(
                "acon",
                selectedFile,
            )
        } else {
            executeCmd(
                "glow",
                selectedFile,
                "-w",
                "0",
            )
        }
        fmt.Printf(
            "%s\n",
            color.N,
        )
    }
}

// Copyright (c) 2026 Zeronetsec