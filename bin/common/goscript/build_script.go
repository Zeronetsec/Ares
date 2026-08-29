// https://github.com/Zeronetsec/Ares

package main

import (
    "bytes"
    "fmt"
    "os"
    "strings"
    "crypto/md5"
    "os/exec"
    "path/filepath"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func buildScript(cfg Config) string {
    var currentMtime string
    if !cfg.IsCode {
        fileInfo, err := os.Stat(cfg.SourceFile)
        if err != nil {
            fmt.Printf(
                "%s[!] %sFailed reading script: %s%v%s\n",
                color.R, color.N, color.GG, err, color.N,
            )
            os.Exit(1)
        }
        currentMtime = fmt.Sprintf(
            "%d",
            fileInfo.ModTime().UnixNano(),
        )
    } else {
        currentMtime = "0"
    }

    prefix := os.Getenv("PREFIX")
    tmpDir := filepath.Join(prefix, "tmp")

    if prefix == "" {
        tmpDir = "/usr/tmp"
        if _, err := os.Stat(tmpDir); err != nil {
            tmpDir = os.TempDir()
        }
    }

    baseCacheDir := filepath.Join(
        tmpDir,
        "goscript_cache",
    )

    var pathHash string
    var fileName string

    if cfg.IsCode {
        pathHash = fmt.Sprintf(
            "%x",
            md5.Sum([]byte(cfg.CodeContent)),
        )
        fileName = "inline_code"
    } else {
        absPath, _ := filepath.Abs(cfg.SourceFile)
        pathHash = fmt.Sprintf(
            "%x",
            md5.Sum([]byte(absPath)),
        )
        fileName = filepath.Base(cfg.SourceFile)
    }

    isolatedDir := filepath.Join(
        baseCacheDir,
        fmt.Sprintf(
            "%s_%s",
            pathHash, fileName,
        ),
    )
    os.MkdirAll(isolatedDir, 0755)

    binaryCache := filepath.Join(
        isolatedDir,
        fmt.Sprintf(
            "%s_%s.bin",
            pathHash, fileName,
        ),
    )

    mtimeFile := binaryCache + ".time"

    cacheValid := false
    if _, err := os.Stat(binaryCache); err == nil {
        if storedMtime, err := os.ReadFile(
            mtimeFile,
        ); err == nil {
            if string(storedMtime) == currentMtime {
                cacheValid = true
            }
        }
    }

    if cacheValid {
        return binaryCache
    }

    tempGoFile := filepath.Join(
        isolatedDir,
        fmt.Sprintf(
            "%s_%s.go",
            pathHash, fileName,
        ),
    )

    varFile := filepath.Join(
        isolatedDir,
        fmt.Sprintf(
            "%s_ghostCode.go",
            pathHash,
        ),
    )

    if cfg.IsCode {
        codeToWrite := cfg.CodeContent
        if !strings.Contains(codeToWrite, "package ") {
            codeToWrite = "package main\n\n" + codeToWrite
        }

        if err := os.WriteFile(
            tempGoFile,
            []byte(codeToWrite),
            0644,
        ); err != nil {
            fmt.Printf(
                "%s[!] %sFailed writing inline code: %s%v%s\n",
                color.R, color.N, color.GG, err, color.N,
            )
            os.Exit(1)
        }
    } else {
        if err := stripShebang(
            cfg.SourceFile,
            tempGoFile,
        ); err != nil {
            fmt.Printf(
                "%s[!] %sFailed processing script: %s%v%s\n",
                color.R, color.N, color.GG, err, color.N,
            )
            os.Exit(1)
        }
    }

    if err := os.WriteFile(
        varFile,
        []byte(ghostCode),
        0644,
    ); err != nil {
        fmt.Printf(
            "%s[!] %sFailed loading: %sghostCode.go %s(%s%v%s)%s\n",
            color.R, color.N, color.GG, color.DG,
            color.GG, err, color.DG, color.N,
        )
        os.Exit(1)
    }

    goModPath := filepath.Join(
        isolatedDir,
        "go.mod",
    )

    modName, err := randomModuleName(16)
    if err != nil {
        modName = "goscript_build"
    }

    if _, err := os.Stat(goModPath); os.IsNotExist(err) {
        initCmd := exec.Command(
            "go",
            "mod",
            "init",
            modName,
        )
        initCmd.Dir = isolatedDir
        _ = initCmd.Run()
    }

    tidyCmd := exec.Command(
        "go",
        "mod",
        "tidy",
    )
    tidyCmd.Dir = isolatedDir

    var errBuf bytes.Buffer
    tidyCmd.Stderr = &errBuf

    if err := tidyCmd.Run(); err != nil {
        fmt.Printf(
            "%s[!] %sFailed to resolve external package!\n",
            color.R, color.N,
        )
        fmt.Printf(
            "%s[!] %sError: %s%s%s\n",
            color.R, color.N, color.GG,
            errBuf.String(), color.N,
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

    cmd.Dir = isolatedDir
    cmd.Stdout = os.Stdout
    cmd.Stderr = os.Stderr

    if err := cmd.Run(); err != nil {
        os.Exit(1)
    }

    os.WriteFile(
        mtimeFile,
        []byte(currentMtime),
        0644,
    )

    return binaryCache
}

// Copyright (c) 2026 Zeronetsec