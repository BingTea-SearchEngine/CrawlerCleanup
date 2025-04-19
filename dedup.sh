#!/bin/bash

# Check for input directory
if [ $# -ne 2 ]; then
    echo "Usage: $0 <link_directory> <output_file>"
    exit 1
fi

LINK_DIR="$1"
OUTPUT="$2"

echo "🔍 Extracting and deduplicating sources from $LINK_DIR..."
awk '{print $1}' "$LINK_DIR"/* | sort | uniq > "$OUTPUT"

echo "✅ Deduplicated sources written to $OUTPUT"
