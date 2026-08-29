// https://github.com/Zeronetsec/Ares

package main

import (
    "crypto/rand"
    "encoding/base64"
)

func randomModuleName(nBytes int) (string, error) {
    b := make([]byte, nBytes)
    _, err := rand.Read(b)
    if err != nil {
        return "", err
    }

    return base64.RawURLEncoding.EncodeToString(b), nil
}

// Copyright (c) 2026 Zeronetsec