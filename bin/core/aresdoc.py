#!/usr/bin/env python3
# https://github.com/Zeronetsec/Ares

# Usage: aresdoc <tool> [--raw]

import os
import re
import subprocess
import sys
from pathlib import Path

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

import color
import missing_argument

def to_snake(name: str) -> str:
    return re.sub(r"([a-z0-9])([A-Z])", r"\1_\2", name).lower()

def to_kebab(name: str) -> str:
    return re.sub(r"([a-z0-9])([A-Z])", r"\1-\2", name).lower()

def find_docs(target_dir: Path, dir_part: str, base_part: str) -> list[str]:
    search_root = target_dir / dir_part
    if not search_root.exists():
        return []

    names = {base_part.lower(), to_snake(base_part), to_kebab(base_part)}
    candidates = {f"{n}.md" for n in names} | {f"{n}.acon" for n in names}

    found = [
        str(p) for p in search_root.rglob("*")
        if p.is_file() and p.name.lower() in candidates
    ]
    return sorted(set(found))

def pick_file(files: list[str], name_input: str) -> str | None:
    if len(files) == 1:
        return files[0]

    result = subprocess.run(
        [
            "fzf",
            "--height=40%",
            "--reverse",
            f"--header=Available documentations for {name_input}:",
        ],
        input="\n".join(files),
        capture_output=True,
        text=True,
    )
    selected = result.stdout.strip()
    return selected or None

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

    found_files = find_docs(target_dir, dir_part, base_part)
    if not found_files:
        print(f"{color.R}[!] {color.N}Documentation for: {color.GG}{name_input} {color.N}not found!")
        sys.exit(1)

    selected_file = pick_file(found_files, name_input)
    if not selected_file:
        print(f"{color.R}[!] {color.N}No document selected!")
        sys.exit(1)

    if use_raw:
        subprocess.run([
            "bat",
            "--color=always",
            "--decorations=never",
            "--paging=never",
            "--language=markdown",
            selected_file,
        ])
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

if __name__ == "__main__":
    main()

# Copyright (c) 2026 Zeronetsec