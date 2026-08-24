// https://github.com/Zeronetsec/Ares

package main

import (
    "fmt"
    "strconv"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func (m model) View() string {
    s := fmt.Sprintf(
        "%s[ (%s%s%s) ]%s\n",
        color.DG, color.GG, m.configPath, color.DG, color.N,
    ) + m.savedStatus + "\n"

    for i, item := range m.items {
        if item.IsComment {
            continue
        }

        if m.cursor == i {
            if m.editing {
                s += fmt.Sprintf(
                    "%s› %s%s %s= %s%s%s\n",
                    color.B, color.RR, item.Key, color.DG,
                    color.B, m.textInput.View(), color.N,
                )
            } else {
                s += fmt.Sprintf(
                    "%s› %s%s %s= %s%s%s\n",
                    color.R, color.B, item.Key, color.DG,
                    color.RR, item.Value, color.N,
                )
            }
        } else {
            typeTag := ""
            if _, err := strconv.ParseBool(item.Value); err == nil {
                typeTag = fmt.Sprintf(
                    " %s[%sboolean%s]%s",
                    color.DG, color.YY, color.DG, color.N,
                )
            } else if _, err := strconv.ParseFloat(item.Value, 64); err == nil {
                typeTag = fmt.Sprintf(
                    " %s[%snumber%s]%s",
                    color.DG, color.B, color.DG, color.N,
                )
            }
            s += fmt.Sprintf(
                "   %s%s %s= %s%s%s%s\n",
                color.WW, item.Key, color.DG,
                color.GG, item.Value, color.N, typeTag,
            )
        }
    }

    if m.editing {
        s += fmt.Sprintf(
            "\n%s[ (%sEdit Mode%s) | %sType %sEdit %s| %sEnter %sSave %s| %sCtrl+E %sCancel %s]%s",
            color.DG, color.B, color.DG, color.GG, color.N,
            color.DG, color.GG, color.N, color.DG, color.GG, color.N,
            color.DG, color.N,
        )
    } else {
        s += fmt.Sprintf(
            "\n%s[ (%sSelect Mode%s) | %s↑/↓ %sMove %s| %sEnter %sEdit %s| %sCtrl+Q %sExit %s]%s",
            color.DG, color.RR, color.DG, color.GG, color.N,
            color.DG, color.GG, color.N,
            color.DG, color.GG, color.N,
            color.DG, color.N,
        )
    }
    s += "\n"

    return s
}

// Copyright (c) 2026 Zeronetsec