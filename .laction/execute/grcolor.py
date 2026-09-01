import re
import sys
from pathlib import Path

script_dir = Path(__file__).resolve().parent
project_root = script_dir.parent.parent

syntax_dir = script_dir / "syntax"

target_list = [
    project_root / "utils" / "color.sh",
    project_root / "lib" / "c" / "color.h",
    project_root / "lib" / "go" / "color" / "color.go",
    project_root / "config" / "acon" / "acon.syntax",
]

def load_color_sc(sc_path: Path) -> dict:
    colors = {}
    if not sc_path.exists():
        return colors

    with open(sc_path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue
            parts = line.split("->")
            if len(parts) == 2:
                key = parts[0].strip()
                val = parts[1].strip()
                colors[key] = val
    return colors

def load_syntax_rules(syntax_path: Path) -> dict:
    rules = {}
    if not syntax_path.exists():
        return rules

    with open(syntax_path, "r", encoding="utf-8") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith("#"):
                continue

            match = re.match(
                r"if\s+(.*?)\s*=\s*_\((.*)\)_",
                line,
            )

            if match:
                extensions_str, template = match.groups()
                exts = [
                    e.strip().lower()
                    for e in extensions_str.split(",")
                ]

                for ext in exts:
                    rules[ext] = template.strip()
    return rules

def process_file(
    file_path: Path,
    colors: dict,
    syntax_rules: dict,
):
    if not file_path.exists():
        print(f"\x1b[1;31m[!] \x1b[0mFile: \x1b[0;32m{file_path} \x1b[0mnot found!")
        return

    ext = file_path.suffix.lstrip(".").lower()
    if ext not in syntax_rules:
        print(f"\x1b[1;31m[!] \x1b[0mSyntax for: \x1b[0;32m.{ext} \x1b[0mnot available!")
        return

    template = syntax_rules[ext]

    with open(file_path, "r", encoding="utf-8") as f:
        content = f.read()

    pattern = re.compile(
        r"([ \t]*)\[action::grcolor::tab::(\d+)\]\s*(.*?)\s*\[end\]",
        re.DOTALL
    )

    def replacer(match):
        indent_spaces = match.group(1)
        tab_num = int(match.group(2))

        if tab_num > 0:
            indent = "    " * tab_num
        else:
            indent = ""

        generated_lines = []
        for color_key, color_val in colors.items():
            line = template.replace(
                "var",
                color_key,
            ).replace("val", color_val)
            generated_lines.append(f"{indent}{line}")

        return "\n".join(generated_lines)

    new_content = pattern.sub(replacer, content)
    if new_content != content:
        with open(
            file_path,
            "w",
            encoding="utf-8"
        ) as f:
            f.write(new_content)
        print(f"\x1b[0;32m[+] \x1b[0mGenerate: \x1b[0;32m{file_path.name}\x1b[0m")
    else:
        print(f"\x1b[0;33m[!] \x1b[0mFile: \x1b[0;32m{file_path.name} \x1b[0mnothing changed detected!")

def main():
    color_sc_path = syntax_dir / "color.sc"
    example_syntax_path = syntax_dir / "example.syntax"

    colors = load_color_sc(color_sc_path)
    syntax_rules = load_syntax_rules(example_syntax_path)

    if not colors:
        print(f"\x1b[1;31m[!] \x1b[0mError load: \x1b[0;32mcolor.sc\x1b[0m")
        sys.exit(1)

    if not syntax_rules:
        print(f"\x1b[1;31m[!] \x1b[0mError load: \x1b[0;32mexample.syntax\x1b[0m")
        sys.exit(1)

    for target in target_list:
        process_file(
            target,
            colors,
            syntax_rules,
        )

if __name__ == "__main__":
    main()