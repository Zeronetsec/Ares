# https://github.com/Zeronetsec/Ares

import re

def execute(name: str) -> str:
    return re.sub(
        r"([a-z0-9])([A-Z])",
        r"\1_\2",
        name,
    ).lower()

# Copyright (c) 2026 Zeronetsec