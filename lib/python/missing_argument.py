# https://github.com/Zeronetsec/Ares

import sys
import os

pylib = os.getenv("__lib__")
sys.path.append(f"{pylib}/python")

import color

def execute(command):
    print(f"{color.R}[!] {color.N}Missing argument!")
    print(f"{color.R}[!] {color.N}Try: {color.GG}aresdoc {color.CC}{command}{color.N}")

# Copyright (c) 2026 Zeronetsec