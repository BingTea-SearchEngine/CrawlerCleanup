#!/bin/bash

# Check if a directory was provided
if [ -z "$1" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

INPUT_DIR="$1"
OUTPUT_FILE="merged_links.txt"
TMP_FILE="__all_lines_tmp.txt"

# Clean up from previous runs
rm -f "$TMP_FILE" "$OUTPUT_FILE"

FILES=("$INPUT_DIR"/*)
TOTAL=${#FILES[@]}
COUNT=0

echo "🔄 Merging files from: $INPUT_DIR"

for file in "${FILES[@]}"; do
    COUNT=$((COUNT + 1))
    echo "📄 Processing ($COUNT/$TOTAL): $file"
    cat "$file" >> "$TMP_FILE"
done

echo "✨ Deduplicating based on the first link in each line..."
awk '!seen[$1]++' "$TMP_FILE" > "$OUTPUT_FILE"

# Clean up
rm -f "$TMP_FILE"

echo "✅ Done! Output saved to $OUTPUT_FILE"

