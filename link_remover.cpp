#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_set>
#include <filesystem>

namespace fs = std::filesystem;
using std::cout, std::endl;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <link directory> <output file>\n";
        return EXIT_FAILURE;
    }

    std::string dirPath = argv[1];
    std::string outPath = argv[2];
    std::string line;

    std::unordered_set<std::string> allSources;

    cout << "🧭 Pass 1: Collecting all source links..." << endl;
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (!entry.is_regular_file()) continue;

        std::ifstream f(entry.path());
        if (!f) {
            std::cerr << "Failed to open " << entry.path() << endl;
            continue;
        }

        while (std::getline(f, line)) {
            std::istringstream stream(line);
            std::string source;
            stream >> source;
            if (!source.empty()) {
                allSources.insert(source);
            }
        }
    }

    cout << "✅ Collected " << allSources.size() << " unique sources.\n";

    std::unordered_set<std::string> seenSources;
    std::ofstream out(outPath);
    if (!out) {
        std::cerr << "Failed to open output file.\n";
        return EXIT_FAILURE;
    }

    cout << "🛠️ Pass 2: Writing cleaned data..." << endl;
    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (!entry.is_regular_file()) continue;

        std::ifstream f(entry.path());
        if (!f) {
            std::cerr << "Failed to open " << entry.path() << endl;
            continue;
        }

        while (std::getline(f, line)) {
            std::istringstream stream(line);
            std::string source;
            stream >> source;

            // Skip if already processed
            if (seenSources.contains(source)) continue;
            seenSources.insert(source);

            out << source;

            std::string dest;
            while (stream >> dest) {
                if (allSources.contains(dest)) {
                    out << " " << dest;
                }
            }

            out << "\n";
        }
    }

    cout << "✅ Done. Output written to " << outPath << endl;
    return 0;
}
