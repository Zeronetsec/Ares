// https://github.com/Zeronetsec/Ares

package main

import (
    "os"
    "fmt"
    "strconv"
    "time"
    "github.com/gdamore/tcell/v2"
    "github.com/Zeronetsec/Ares/lib/go/color"
)

func main() {
    s, err := tcell.NewScreen()
    if err != nil {
        fmt.Fprintf(
            os.Stderr,
            "%s[!] %sError: %s%v%s\n",
            color.R, color.N, color.GG, err, color.N,
        )
        os.Exit(1)
    }

    if err := s.Init(); err != nil {
        fmt.Fprintf(
            os.Stderr,
            "%s[!] %sError: %s%v%s\n",
            color.R, color.N, color.GG, err, color.N,
        )
        os.Exit(1)
    }

    defer s.Fini()

    styleNormal := tcell.StyleDefault.Foreground(
        tcell.ColorWhite,
    ).Background(tcell.ColorReset)

    styleGreen := tcell.StyleDefault.Foreground(
        tcell.ColorGreen,
    ).Background(tcell.ColorReset)

    styleCyan := tcell.StyleDefault.Foreground(
        tcell.ColorTeal,
    ).Background(tcell.ColorReset)

    styleYellow := tcell.StyleDefault.Foreground(
        tcell.ColorYellow,
    ).Background(tcell.ColorReset)

    styleRedBold := tcell.StyleDefault.Foreground(
        tcell.ColorRed,
    ).Background(tcell.ColorReset).Bold(true)

    styleDarkGray := tcell.StyleDefault.Foreground(
        tcell.ColorDarkGray,
    ).Background(tcell.ColorReset).Bold(true)

    styleTitle := tcell.StyleDefault.Foreground(
        tcell.ColorWhite,
    ).Background(tcell.ColorReset).Bold(true)

    quit := make(chan struct{})
    go func() {
        for {
            ev := s.PollEvent()
            switch ev := ev.(type) {
                case *tcell.EventKey:
                    if ev.Key() == tcell.KeyCtrlQ {
                        close(quit)
                        return
                    }
                case *tcell.EventResize:
                    s.Sync()
            }
        }
    }()

    ticker := time.NewTicker(RefreshMs * time.Millisecond)
    defer ticker.Stop()

    for {
        s.Clear()
        w, h := s.Size()

        colPid := 7
        colCpu := 6
        colMem := 6
        colCmd := w - (colPid + colCpu + colMem + 5)
        if colCmd < 10 {
            colCmd = 10
        }

        procs := getProcesses()

        drawString(s, 0, 0, styleTitle, "ATOP ")
        drawString(s, 5, 0, styleNormal, "(press ")
        drawString(s, 12, 0, styleGreen, "Ctrl+Q")
        drawString(s, 18, 0, styleNormal, " to quit)")

        headerStr := fmt.Sprintf(
            "%*s %-*s %*s %*s",
            colPid, "PID",
            colCmd, "CMD",
            colCpu, "CPU%",
            colMem, "MEM%",
        )
        drawString(s, 0, 2, styleNormal, headerStr)

        lineStr := ""
        for i := 0; i < w-1; i++ {
            lineStr += "─"
        }
        drawString(s, 0, 3, styleDarkGray, lineStr)

        for i, p := range procs {
            if i+4 >= h {
                break
            }

            pidStr := strconv.Itoa(int(p.PID))
            cmdStr := p.Name
            if len(cmdStr) > colCmd {
                cmdStr = cmdStr[:colCmd]
            }

            cpuStr := fmt.Sprintf("%.1f", p.CPU)
            memStr := fmt.Sprintf("%.1f", p.Memory)

            var cpuStyle tcell.Style
            if p.CPU > 50 {
                cpuStyle = styleRedBold
            } else if p.CPU > 20 {
                cpuStyle = styleYellow
            } else {
                cpuStyle = styleCyan
            }

            y := i + 4
            drawString(s, 0, y, styleRedBold, "› ")

            currentX := 2
            drawString(
                s, currentX, y, styleNormal,
                fmt.Sprintf(
                    "%*s ", colPid, pidStr,
                ),
            )
            currentX += colPid + 1

            drawString(
                s, currentX, y, styleGreen,
                fmt.Sprintf(
                    "%-*s ", colCmd, cmdStr,
                ),
            )
            currentX += colCmd + 1

            drawString(
                s, currentX, y, cpuStyle,
                fmt.Sprintf(
                    "%*s ", colCpu, cpuStr,
                ),
            )
            currentX += colCpu + 1

            drawString(
                s, currentX, y, styleCyan,
                fmt.Sprintf(
                    "%*s", colMem, memStr,
                ),
            )
        }

        s.Show()
        select {
            case <-quit:
                return
            case <-ticker.C:
        }
    }
}

// Copyright (c) 2026 Zeronetsec