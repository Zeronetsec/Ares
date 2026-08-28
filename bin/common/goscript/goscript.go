// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "os"
    "syscall"
    "bytes"
    "crypto/md5"
    "os/exec"
    "path/filepath"
    "github.com/Zeronetsec/Ares/lib/go/color"
    "github.com/Zeronetsec/Ares/lib/go/invinput"
)

func main() {
    if len(os.Args) < 2 {
        invinput.MissingArgument("goscript")
        os.Exit(1)
    }

    sourceFile := os.Args[1]
    extraArgs := os.Args[2:]

    fileInfo, err := os.Stat(sourceFile)
    if err != nil {
        fmt.Printf(
            "%s[!] %sFailed reading script: %s%v%s\n",
            color.R, color.N, color.GG, err, color.N,
        )
        os.Exit(1)
    }

    prefix := os.Getenv("PREFIX")
    tmpDir := filepath.Join(prefix, "tmp")

    if prefix == "" {
        tmpDir = "/usr/tmp"
        if _, err := os.Stat(tmpDir); err != nil {
            tmpDir = os.TempDir()
        }
    }

    cacheDir := filepath.Join(tmpDir, "goscript_cache")
    os.MkdirAll(cacheDir, 0755)

    absPath, _ := filepath.Abs(sourceFile)
    pathHash := fmt.Sprintf(
        "%x", md5.Sum(
            []byte(absPath),
        ),
    )

    fileName := filepath.Base(sourceFile)
    binaryCache := filepath.Join(
        cacheDir, fmt.Sprintf(
            "%s_%s.bin",
            pathHash, fileName,
        ),
    )

    mtimeFile := binaryCache + ".time"
    currentMtime := fmt.Sprintf(
        "%d", fileInfo.ModTime().UnixNano(),
    )

    cacheValid := false

    if _, err := os.Stat(binaryCache); err == nil {
        if storedMtime, err := os.ReadFile(mtimeFile); err == nil {
            if string(storedMtime) == currentMtime {
                cacheValid = true
            }
        }
    }

    if !cacheValid {
        tempGoFile := filepath.Join(
            cacheDir, fmt.Sprintf(
                "%s_%s.go",
                pathHash, fileName,
            ),
        )

        varFile := filepath.Join(
            cacheDir, fmt.Sprintf(
                "%s_ghostCode.go",
                pathHash,
            ),
        )

        if err := stripShebang(sourceFile, tempGoFile); err != nil {
            fmt.Printf(
                "%s[!] %sFailed processing script: %s%v%s\n",
                color.R, color.N, color.GG, err, color.N,
            )
            os.Exit(1)
        }

        if err := os.WriteFile(
            varFile, []byte(ghostCode), 0644,
        ); err != nil {
            fmt.Printf(
                "%s[!] %sFailed loading ghostCode.go: %s%v%s\n",
                color.R, color.N, color.GG, err, color.N,
            )
            os.Exit(1)
        }

        goModPath := filepath.Join(cacheDir, "go.mod")
        if _, err := os.Stat(goModPath); os.IsNotExist(err) {
            initCmd := exec.Command(
                "go",
                "mod",
                "init",
                "goscript_cache",
            )

            initCmd.Dir = cacheDir
            _ = initCmd.Run()
        }

        tidyCmd := exec.Command(
            "go",
            "mod",
            "tidy",
        )

        tidyCmd.Dir = cacheDir

        var errBuf bytes.Buffer
        tidyCmd.Stderr = &errBuf

        if err := tidyCmd.Run(); err != nil {
            fmt.Printf(
                "%s[!] %sFailed to resolve external package!\n",
                color.R, color.N,
            )

            fmt.Printf(
                "%s[!] %sError: %s%s%s\n",
                color.R, color.N, color.GG, errBuf.String(), color.N,
            )
            os.Exit(1)
        }

        cmd := exec.Command(
            "go",
            "build",
            "-o",
            binaryCache,
            filepath.Base(tempGoFile),
            filepath.Base(varFile),
        )

        cmd.Dir = cacheDir
        cmd.Stdout = os.Stdout
        cmd.Stderr = os.Stderr

        if err := cmd.Run(); err != nil {
            os.Exit(1)
        }

        os.WriteFile(
            mtimeFile, []byte(currentMtime), 0644,
        )
    }

    binaryArgs := append(
        []string{binaryCache}, extraArgs...,
    )

    err = syscall.Exec(
        binaryCache, binaryArgs, os.Environ(),
    )
    if err != nil {
        fmt.Printf(
            "%s[!] %sFailed to execute binary: %s%v%s\n",
            color.R, color.N, color.GG, err, color.N,
        )
        os.Exit(1)
    }
}

// Copyright (c) 2026 Zeronetsec