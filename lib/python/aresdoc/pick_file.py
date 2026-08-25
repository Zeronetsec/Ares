# https://github.com/Zeronetsec/Ares

import sys
import os

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

import subprocess

def execute(
    files: list[str],
    name_input: str,
) -> str | None:
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

# Copyright (c) 2026 Zeronetsec