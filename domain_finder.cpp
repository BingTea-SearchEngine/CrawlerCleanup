#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <cstdio>

namespace fs = std::filesystem;

std::string getUrl(fs::path &filepath){
    std::ifstream inFile(filepath);
    if (!inFile.is_open()){
        return "";
    }

    std::string line;
    std::getline(inFile, line);
    std::string before = "URL: ";
    std::string after = " Doc number:";
    size_t s = line.find(before);
    if (s == std::string::npos){
        return "";
    }
    s += before.size();
    size_t e = line.find(after, s);
    if (e == std::string::npos){
        return "";
    }
    return line.substr(s, e - s);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Expected: " << argv[0] << " [-d] <base_url> <directory_path>\n";
        return 1;
    }

    bool shouldDelete = false;
    std::string baseurl;
    std::string dirpath;

    if (std::string(argv[1]) == "-d") {
        if (argc < 4) {
            std::cerr << "Needs base_url and dirpath after -d\n";
            return 1;
        }
        shouldDelete = true;
        baseurl = argv[2];
        dirpath = argv[3];
    }
    else {
        baseurl = argv[1];
        dirpath = argv[2];
    }

    std::vector<std::pair<std::string, std::string>> matches;

    size_t total = 0;
    size_t matched = 0;

    for (const auto &entry : fs::directory_iterator(dirpath)) {
        if (!entry.is_regular_file()) continue;
        auto path = entry.path();
        if (path.extension() == ".parsed") {
            total++;
            std::string docurl = getUrl(path);
            if (!docurl.empty()){
                if (docurl.rfind(baseurl, 0) == 0) {
                    matched++;
                    matches.push_back({docurl, path.filename().string()});
                }
            }
        }
    }

    for (auto &p : matches) {
        std::cout << p.first << ": " << p.second << "\n";
    }
    std::cout << "Number of document matches: " << matched << "\n";

    if (shouldDelete && matched > 0) {
        size_t removed = 0;
        for (auto &p : matches) {
            std::string fullpath = (fs::path(dirpath) / p.second).string();
            if (std::remove(fullpath.c_str()) == 0) {
                removed++;
            } else {
                std::perror(("Error removing file " + fullpath).c_str());
            }
        }
        std::cout << "\nOriginal document count: " << total << "\n"
                  << "Documents removed: " << removed << "\n"
                  << "Final document count: " << (total - removed) << "\n";
    }
    return 0;
}