// https://github.com/Zeronetsec/Ares

package main

import (
    "sort"
    "github.com/shirou/gopsutil/v3/process"
)

func getProcesses() []ProcInfo {
    procs, err := process.Processes()
    if err != nil {
        return nil
    }

    var procList []ProcInfo
    for _, p := range procs {
        name, err := p.Name()
        if err != nil {
            continue
        }

        cpu, err := p.CPUPercent()
        if err != nil {
            continue
        }

        mem, err := p.MemoryPercent()
        if err != nil {
            continue
        }

        procList = append(procList, ProcInfo{
            PID: p.Pid,
            Name: name,
            CPU: cpu,
            Memory: mem,
        })
    }

    sort.Slice(procList, func(i, j int) bool {
        return procList[i].CPU > procList[j].CPU
    })

    if len(procList) > MaxProcs {
        return procList[:MaxProcs]
    }

    return procList
}

// Copyright (c) 2026 Zeronetsec