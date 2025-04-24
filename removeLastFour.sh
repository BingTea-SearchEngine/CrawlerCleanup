#!/bin/bash

DIR="$1"

if [ -z "$DIR" ]; then
    echo "Usage: $0 <directory>"
    exit 1
fi

for file in "$DIR"/*; do
    if [ -f "$file" ]; then
        total_lines=$(wc -l < "$file")
        lines_to_keep=$((total_lines - 8))
    
        if [ "$lines_to_keep" -gt 0 ]; then
            sed "1,${lines_to_keep}p;d" "$file" > "$file.tmp"
            mv "$file.tmp" "$file"
            echo "Trimmed: $file"
        else
            echo "Skipped (less than 4 lines): $file"
        fi
    fi
done
