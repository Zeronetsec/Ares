// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "strconv"
)

func (m model) View() string {
    s := fmt.Sprintf(
        "%s[ (%s%s%s) ]%s\n",
        DG, GG, m.configPath, DG, N,
    ) + m.savedStatus + "\n"

    for i, item := range m.items {
        if item.IsComment {
            continue
        }

        if m.cursor == i {
            if m.editing {
                s += fmt.Sprintf(
                    "%s› %s%s %s= %s%s%s\n",
                    B, RR, item.Key, DG, B, m.textInput.View(), N,
                )
            } else {
                s += fmt.Sprintf(
                    "%s› %s%s %s= %s%s%s\n",
                    R, B, item.Key, DG, RR, item.Value, N,
                )
            }
        } else {
            typeTag := ""
            if _, err := strconv.ParseBool(item.Value); err == nil {
                typeTag = fmt.Sprintf(
                    " %s[%sboolean%s]%s",
                    DG, YY, DG, N,
                )
            } else if _, err := strconv.ParseFloat(item.Value, 64); err == nil {
                typeTag = fmt.Sprintf(
                    " %s[%snumber%s]%s",
                    DG, B, DG, N,
                )
            }
            s += fmt.Sprintf(
                "   %s%s %s= %s%s%s%s\n",
                WW, item.Key, DG, GG, item.Value, N, typeTag,
            )
        }
    }

    if m.editing {
        s += fmt.Sprintf(
            "\n%s[ (%sEdit Mode%s) | %sType %sEdit %s| %sEnter %sSave %s| %sCtrl+E %sCancel %s]%s",
            DG, B, DG, GG, N, DG, GG, N, DG, GG, N, DG, N,
        )
    } else {
        s += fmt.Sprintf(
            "\n%s[ (%sSelect Mode%s) | %s↑/↓ %sMove %s| %sEnter %sEdit %s| %sCtrl+Q %sExit %s]%s",
            DG, RR, DG, GG, N, DG, GG, N, DG, GG, N, DG, N,
        )
    }
    s += "\n"

    return s
}

// Copyright (c) 2026 Zeronetsec