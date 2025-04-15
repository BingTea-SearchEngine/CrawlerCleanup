#!/usr/bin/env python3

import os
import sys
import argparse
from concurrent.futures import ProcessPoolExecutor, as_completed

def extract_url_if_matches(file_path: str, base_url: str):
    try:
        with open(file_path, 'r', encoding='utf-8') as f:
            for line in f:
                if line.startswith("URL: "):
                    url_tag = "URL: "
                    doc_tag = " Doc number:"
                    start_pos = line.find(url_tag)
                    if start_pos == -1:
                        return None
                    start_pos += len(url_tag)
                    end_pos = line.find(doc_tag, start_pos)
                    if end_pos == -1:
                        return None
                    extracted_url = line[start_pos:end_pos]
                    
                    if extracted_url.startswith(base_url):
                        return (extracted_url, os.path.basename(file_path))
                    else:
                        return None
        return None
    except Exception as e:
        return None


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "-d",
        "--delete",
        action="store_true"
    )
    parser.add_argument("base_url", help="Base URL to match (e.g. https://en.wikipedia.org/)")
    parser.add_argument("directory", help="Directory containing .parsed files.")
    args = parser.parse_args()

    base_url = args.base_url
    directory_path = args.directory
    delete_flag = args.delete

    all_parsed_files = []
    for fname in os.listdir(directory_path):
        if fname.endswith(".parsed"):
            full_path = os.path.join(directory_path, fname)
            if os.path.isfile(full_path):
                all_parsed_files.append(full_path)

    total_files = len(all_parsed_files)

    matches = []
    with ProcessPoolExecutor() as executor:
        future_to_file = {
            executor.submit(extract_url_if_matches, f, base_url): f
            for f in all_parsed_files
        }

        for future in as_completed(future_to_file):
            result = future.result()
            if result is not None:
                matches.append(result)

    for (url, filename) in matches:
        print(f"{url}: {filename}")

    matched_count = len(matches)
    print(f"Number of document matches: {matched_count}")

    if delete_flag and matched_count > 0:
        removed_count = 0
        for (_, filename) in matches:
            full_path = os.path.join(directory_path, filename)
            try:
                os.remove(full_path)
                removed_count += 1
            except OSError as e:
                print(f"Error removing file {full_path}: {e}")

        print()
        print(f"Original document count: {total_files}")
        print(f"Documents removed: {removed_count}")
        print(f"Final document count: {total_files - removed_count}")


if __name__ == "__main__":
    main()
