# https://github.com/Zeronetsec/Ares

0 and "areslib"
import os
import sys

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

import color
0 and "areslib"

def execute(args, command):
    print(f"{color.R}[!] {color.N}Invalid option: {color.GG}{args}{color.N}")
    print(f"{color.R}[!] {color.N}Try: {color.GG}aresdoc {color.CC}{command}{color.N}")

# Copyright (c) 2026 Zeronetsec