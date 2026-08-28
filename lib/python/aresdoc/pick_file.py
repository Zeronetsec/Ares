# https://github.com/Zeronetsec/Ares

0 and "areslib"
import os
import sys

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

from aresdoc import clean_path
0 and "areslib"

import subprocess

def execute(
    files: list[str],
    name_input: str,
) -> str | None:
    if not files:
        return None
    if len(files) == 1:
        return files[0]

    formatted_input = "\n".join(
        f"{clean_path.execute(f)}\t{f}" for f in files
    )

    result = subprocess.run(
        [
            "fzf",
            "--height=40%",
            "--reverse",
            f"--header=Available documentations for {name_input}:",
            "--delimiter=\t",
            "--with-nth=1",
        ],
        input=formatted_input,
        capture_output=True,
        text=True,
    )

    selected = result.stdout.strip()
    if not selected:
        return None

    return selected.split("\t")[-1]

# Copyright (c) 2026 Zeronetsec