import os
import re

# folder to process
root_folder = "./"

# regex to match: VAR = LoadTexture(FILENAME);
pattern = re.compile(r'(\w+)\s*=\s*LoadTexture\(([^)]+)\);')

def process_file(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        lines = f.readlines()

    changed = False
    new_lines = []
    for line in lines:
        match = pattern.search(line)
        if match:
            var = match.group(1)
            file_arg = match.group(2)
            new_line = f"if (!IsTextureValid({var})){{\n    {var} = LoadTexture({file_arg});\n}}\n"
            new_lines.append(new_line)
            changed = True
        else:
            new_lines.append(line)

    if changed:
        with open(filepath, "w", encoding="utf-8") as f:
            f.writelines(new_lines)
        print(f"Updated: {filepath}")

def process_folder(folder):
    for root, dirs, files in os.walk(folder):
        for file in files:
            if file.endswith((".c", ".cpp", ".h")):
                process_file(os.path.join(root, file))

process_folder(root_folder)
