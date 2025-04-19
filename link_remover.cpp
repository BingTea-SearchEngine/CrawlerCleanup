#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <link_directory> <deduped_sources.txt>\n";
        return EXIT_FAILURE;
    }

    std::string dirPath = argv[1];
    std::string sourceFilePath = argv[2];
    std::string outputFilePath = "cleaned_links.txt";

    std::unordered_set<std::string> validSources;
    std::unordered_set<std::string> writtenSources;

    std::ifstream sourceFile(sourceFilePath);
    std::string line;

    if (!sourceFile) {
        std::cerr << "Failed to open deduped sources file.\n";
        return EXIT_FAILURE;
    }

    // Load valid sources
    while (std::getline(sourceFile, line)) {
        if (!line.empty()) {
            validSources.insert(line);
        }
    }

    std::ofstream outFile(outputFilePath);
    if (!outFile) {
        std::cerr << "Failed to open output file for writing.\n";
        return EXIT_FAILURE;
    }

    std::cout << "🚀 Processing files in: " << dirPath << std::endl;
    for (const auto& file : fs::directory_iterator(dirPath)) {
        if (!file.is_regular_file()) continue;

        std::ifstream in(file.path());
        if (!in) {
            std::cerr << "Failed to read: " << file.path() << std::endl;
            continue;
        }

        while (std::getline(in, line)) {
            std::istringstream iss(line);
            std::string source;
            iss >> source;

            // Skip if not a valid source or already written
            if (!validSources.count(source) || writtenSources.count(source)) continue;
            writtenSources.insert(source);

            outFile << source;
            std::string dest;
            while (iss >> dest) {
                if (validSources.count(dest)) {
                    outFile << " " << dest;
                }
            }
            outFile << "\n";
        }
    }

    std::cout << "✅ Finished. Output saved to " << outputFilePath << std::endl;
    return 0;
}
