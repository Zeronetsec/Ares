// https://github.com/Zeronetsec/Ares

package main

import (
    "github.com/charmbracelet/bubbles/textinput"
    tea "github.com/charmbracelet/bubbletea"
)

func (m model) Init() tea.Cmd {
    return textinput.Blink
}

// Copyright (c) 2026 Zeronetsec