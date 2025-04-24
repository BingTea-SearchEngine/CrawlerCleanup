import os
import sys

PRANK = """<prank>
1
</prank>
"""
CRANK = """<crank>
1
</crank>
"""

COMMUNITY = """<community>
101
</comunity>
<communitycount>
3000
</communitycount>
"""

def append_if_missing(file_path):
    try:
        appendText = ""
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
            if "<prank>" not in content:
                appendText += PRANK
            if "<crank>" not in content:
                appendText += CRANK
            if "<community>" not in content:
                appendText+=COMMUNITY

            if appendText == "":
                print(f"Skipping {file_path}")
                return

        with open(file_path, 'a', encoding='utf-8') as f:
            f.write(appendText)
        print(f"Appended to {file_path}")
    except Exception as e:
        print(f"Failed to process {file_path}: {e}")

def process_directory(directory):
    if not os.path.isdir(directory):
        print(f"Error: {directory} is not a valid directory.")
        return

    for filename in os.listdir(directory):
        file_path = os.path.join(directory, filename)
        if os.path.isfile(file_path):
            append_if_missing(file_path)

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Usage: python append_tags_if_missing.py <directory_path>")
        sys.exit(1)

    directory = sys.argv[1]
    process_directory(directory)

