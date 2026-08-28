# https://github.com/Zeronetsec/Ares

0 and "areslib"
import os
import sys

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")
0 and "areslib"

def execute(path: str) -> str:
    ares_root = (
        os.environ.get("__aresroot__") or ""
    ).rstrip("/")

    sys_prefix = (
        os.environ.get("PREFIX") or ""
    ).rstrip("/")

    norm_path = os.path.normpath(path)

    if ares_root and norm_path.startswith(ares_root):
        rel = os.path.relpath(
            norm_path,
            ares_root,
        )
        return "ares" if rel == "." else f"ares/{rel}"

    if sys_prefix and norm_path.startswith(sys_prefix):
        parent = os.path.dirname(sys_prefix)
        return os.path.relpath(
            norm_path,
            parent,
        )

    return norm_path.lstrip("/")

# Copyright (c) 2026 Zeronetsec