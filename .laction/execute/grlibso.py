import re
import sys
from pathlib import Path

script_dir = Path(__file__).resolve().parent
project_root = script_dir.parent.parent

respath = "lib/shell/libso"

target_dirs = [
    project_root / "utils" / "libso",
]

target_files = [
    project_root / "init" / "loader.sh",
    project_root / "lib" / "shell" / "libso" / "init.init",
]

def collect_library_mappings(
    dirs: list[Path],
    base_respath: str,
) -> dict:
    mappings = {}
    clean_respath = base_respath.strip("/")

    for d in dirs:
        if not d.exists() or not d.is_dir():
            print(f"\x1b[1;31m[!] \x1b[0mDirectory: \x1b[0;32m{d} \x1b[0mnot found!")
            continue

        for file_path in d.rglob("*"):
            if file_path.is_file():
                basename = file_path.stem
                formatted_path = f"{clean_respath}/{basename}"
                mappings[formatted_path] = basename

    return mappings

def process_file(file_path: Path, mappings: dict):
    if not file_path.exists():
        print(f"\x1b[1;31m[!] \x1b[0mFile: \x1b[0;32m{file_path} \x1b[0mnot found!")
        return

    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    pattern = re.compile(
        r"([ \t]*)\[action::grlibso::tab::(\d+)\]\s*(.*?)\s*\[end\]",
        re.DOTALL
    )

    def replacer(match):
        tab_num = int(match.group(2))
        indent = "    " * tab_num if tab_num > 0 else ""

        generated_lines = [
            f"{indent}{path_key} -> {basename_val}"
            for path_key, basename_val in mappings.items()
        ]

        return "\n".join(generated_lines)

    new_content = pattern.sub(replacer, content)
    if new_content != content:
        with open(file_path, "w", encoding="utf-8") as f:
            f.write(new_content)
        print(f"\x1b[0;32m[+] \x1b[0mGenerated: \x1b[0;32m{file_path.name}\x1b[0m")
    else:
        print(f"\x1b[0;33m[!] \x1b[0mFile: \x1b[0;32m{file_path.name} \x1b[0mnothing changed!")

def main():
    mappings = collect_library_mappings(target_dirs, respath)
    if not mappings:
        print("\x1b[1;31m[!] \x1b[0mNo files found in target directories!")
        sys.exit(1)

    for target in target_files:
        process_file(target, mappings)

if __name__ == "__main__":
    main()