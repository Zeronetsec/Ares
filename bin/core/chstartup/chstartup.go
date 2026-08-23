// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "os"
    "github.com/charmbracelet/bubbles/textinput"
    tea "github.com/charmbracelet/bubbletea"
)

func main() {
    path := os.Getenv("__config__") + "/startup.conf"
    items, err := readCfg(path)
    if err != nil {
        fmt.Printf(
            "%s[!] %sFailed reading startup config: %s%v%s\n",
            R, N, GG, err, N,
        )
        os.Exit(1)
    }

    ti := textinput.New()
    ti.Focus()
    ti.CharLimit = 50
    ti.Width = 20

    m := model{
        items: items,
        textInput: ti,
        configPath: path,
    }

    p := tea.NewProgram(m, tea.WithAltScreen())
    if _, err := p.Run(); err != nil {
        fmt.Printf(
            "%s[!] %sError TUI: %s%v%s\n",
                R, N, GG, err, N,
        )
        os.Exit(1)
    }
}

// Copyright (c) 2026 Zeronetsec