import os
import argparse

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("directory", help="Directory containing .parsed files.")
    args = parser.parse_args()

    dir = args.directory
    NUM_FILES = 1000
    urls = ["https://google.com", "https://wikipedia.org", "https://twitter.com",
            "https://wikipedia.org/"]

    file_num = 0
    for url in urls:
        for i in range(NUM_FILES):
            filepath = dir + f"{file_num}.parsed"
            with open(filepath, "w") as file:
                file.write(f"URL: {url} Doc number: {file_num}")
            file_num+=1

    print(file_num, "files")
    print(len(urls), "unique files")

