#!/usr/bin/env python3
# https://github.com/Zeronetsec/Ares

# Usage: atoolkit [--allpath] [--only <path>]

import sys
import os

0 and "areslib"
pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

import color
import invalid_option
from atoolkit import get_clean_path
from atoolkit import scandir
0 and "areslib"

def main():
    argv = sys.argv[1:]
    allpath = False
    only = None
    i = 0
    valid = True

    while i < len(argv):
        arg = argv[i]
        if arg == "--allpath":
            allpath = True
            i += 1
        elif arg == "--only":
            if i + 1 < len(argv):
                only = argv[i + 1]
                i += 2
            else:
                valid = False
                break
        else:
            invalid_option.execute(
                arg,
                "atoolkit",
            )
            sys.exit(1)

    source = os.environ.get(
        "PATH",
    ) if allpath else os.environ.get(
        "__bin__",
    )

    if not source and not allpath:
        source = os.environ.get("PATH")

    if not source:
        print(
            f"{color.R}[!] {color.N}Environment variable not found!",
            file=sys.stderr,
        )
        sys.exit(1)

    clean_filter = only.strip("/") if only else None
    paths = [
        p for p in source.split(":") if p
    ]
    found_any = False

    for p in paths:
        if scandir.execute(p, clean_filter):
            found_any = True

    if clean_filter and not found_any:
        print(
            f"{color.R}[!] {color.N}Path: {color.GG}{only} {color.N}not found!",
            file=sys.stderr,
        )

if __name__ == "__main__":
    main()

# Copyright (c) 2026 Zeronetsec