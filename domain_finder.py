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
                        os.remove(file_path)
                        return True
                    return False
        return False
    except Exception as e:
        return False


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

    deleted = 0
    with ProcessPoolExecutor() as executor:
        future_to_file = {
            executor.submit(extract_url_if_matches, f, base_url): f
            for f in all_parsed_files
        }

        for future in as_completed(future_to_file):
            result = future.result()
            if result:
                deleted+=1

    print(f"Original document count: {total_files}")
    print(f"Documents removed: {deleted}")
    print(f"Final document count: {total_files - deleted}")


if __name__ == "__main__":
    main()
