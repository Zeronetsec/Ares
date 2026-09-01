// https://github.com/Zeronetsec/Ares

package main

import (
    "regexp"
)

var camelCaseRe = regexp.MustCompile(
    `([a-z0-9])([A-Z])`,
)

// Copyright (c) 2026 Zeronetsec