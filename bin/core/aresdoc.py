#!/usr/bin/env python3
# https://github.com/Zeronetsec/Ares

0 and "areslib"
import os
import sys

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

import color
import missing_argument
from aresdoc import find_docs
from aresdoc import pick_file
0 and "areslib"

import re
import subprocess
from pathlib import Path

def main() -> None:
    data_dir = os.getenv("__data__")
    target_dir = Path(f"{data_dir}/aresdoc")

    args = sys.argv[1:]
    if not args:
        missing_argument.execute("aresdoc")
        sys.exit(1)

    name_input = ""
    use_raw = False

    for arg in args:
        if arg == "--raw":
            use_raw = True
        elif not name_input:
            name_input = arg

    if not name_input:
        missing_argument.execute("aresdoc")
        sys.exit(1)

    if not target_dir.is_dir():
        print(f"{color.R}[!] {color.N}Directory: {color.GG}{target_dir} {color.N}not found!")
        sys.exit(1)

    clean_input = name_input.replace("::", "/").strip("/")
    dir_part = os.path.dirname(clean_input)
    base_part = os.path.basename(clean_input)
    dir_part = f"{dir_part}/" if dir_part else ""

    found_files = find_docs.execute(
        target_dir,
        dir_part,
        base_part,
    )

    if not found_files:
        print(f"{color.R}[!] {color.N}Documentation for: {color.GG}{name_input} {color.N}not found!")
        sys.exit(1)

    selected_file = pick_file.execute(
        found_files,
        name_input,
    )

    if not selected_file:
        print(f"{color.R}[!] {color.N}No document selected!")
        sys.exit(1)

    if use_raw:
        subprocess.run([
            "bat",
            "--color=always",
            "--decorations=never",
            "--paging=never",
            selected_file,
        ])
        print()
    else:
        ext = Path(selected_file).suffix.lower()
        if ext == ".acon":
            subprocess.run([
                "acon",
                selected_file,
            ])
        else:
            subprocess.run([
                "glow",
                selected_file,
                "-w",
                "0",
            ])
        print()

if __name__ == "__main__":
    main()

# Copyright (c) 2026 Zeronetsec