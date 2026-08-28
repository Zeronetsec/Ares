# https://github.com/Zeronetsec/Ares

0 and "areslib"
import os
import sys

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

from aresdoc import to_snake
from aresdoc import to_kebab
0 and "areslib"

from pathlib import Path

def execute(
    target_dir: Path,
    dir_part: str,
    base_part: str,
) -> list[str]:
    search_root = target_dir / dir_part
    if not search_root.exists():
        return []

    names = {
        base_part.lower(),
        to_snake.execute(base_part),
        to_kebab.execute(base_part),
    }

    candidates = {
        f"{n}.md" for n in names
    } | {
        f"{n}.acon" for n in names
    }

    found = [
        str(p) for p in search_root.rglob("*")
        if p.is_file() and p.name.lower() in candidates
    ]

    return sorted(set(found))

# Copyright (c) 2026 Zeronetsec