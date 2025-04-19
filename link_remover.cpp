#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

using std::cout, std::endl;

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "Expected: " << argv[0] << "<link directory> <out file path>\n";
        exit(EXIT_FAILURE);
    }
    std::string dirPath = std::string(argv[1]);
    std::string outPath = std::string(argv[2]);
    std::string line;

    std::unordered_map<std::string, uint8_t> sources;

    cout << "Getting source links" << endl;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (!entry.is_regular_file()) continue;

        std::ifstream f(entry.path());

        cout << "Starting " << entry.path() << "..." << endl;
        while (std::getline(f, line)) {
            std::istringstream stream(line);
            std::string sourceUrl;
            stream >> sourceUrl;
            sources.insert({sourceUrl, 0});
        }
        cout << "... Finished " << entry.path() << endl;
    }

    cout << sources.size() << endl;
    std::ofstream o(outPath);

    cout << "Writing to file" << endl;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        std::ifstream f(entry.path());
        cout << "Starting " << entry.path() << "..." << endl;
        while (std::getline(f, line)) {
            std::istringstream stream(line);
            std::string sourceUrl;
            stream >> sourceUrl;
            if (sources[sourceUrl] == 1) continue;
            o << sourceUrl << " ";
            sources[sourceUrl]++;

            std::string url;
            while (stream >> url) {
                if (sources.find(url) == sources.end()) continue;
                o << url << " ";
            }
            o << "\n";
        }
        cout << "... Finished " << entry.path() << endl;
    }
}
