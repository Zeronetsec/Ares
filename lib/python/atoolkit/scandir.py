# https://github.com/Zeronetsec/Ares

import os
import sys

0 and "areslib->"
pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

from atoolkit import get_clean_path
from atoolkit import scandir
0 and "<-areslib"

visited_dirs = set()

def execute(
    path: str,
    clean_filter: str | None = None,
) -> bool:
    try:
        real_path = os.path.realpath(path)
    except OSError:
        return False

    if real_path in visited_dirs or not os.path.isdir(real_path):
        return False

    visited_dirs.add(real_path)

    clean_p = get_clean_path.execute(path)
    next_filter = clean_filter
    match = True

    if clean_filter:
        if clean_p.startswith(clean_filter):
            match = True
            next_filter = None
        elif clean_filter.startswith(clean_p):
            match = False
            next_filter = clean_filter
        else:
            return False

    executables = []
    subdirs = []

    try:
        with os.scandir(real_path) as entries:
            for entry in entries:
                if entry.name in ('.', '..'):
                    continue
                if entry.is_file(
                    follow_symlinks=True,
                ) and os.access(
                    entry.path,
                    os.X_OK,
                ):
                    executables.append(entry.name)
                elif entry.is_dir(
                    follow_symlinks=True,
                ):
                    subdirs.append(entry.path)
    except PermissionError:
        return False

    found = False
    if executables and match:
        executables.sort()
        print(f"{clean_p}:")
        for exe in executables:
            print(f"› {exe}")
        print()
        found = True

    for sdir in sorted(subdirs):
        if scandir.execute(
            sdir,
            next_filter,
        ):
            found = True

    return found

# Copyright (c) 2026 Zeronetsec