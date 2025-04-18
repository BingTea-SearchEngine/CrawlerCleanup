#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <cstdio>

namespace fs = std::filesystem;

void getLinks(fs::path &filepath){
    std::ifstream inFile(filepath);
    if (!inFile.is_open()){
        return;
    }

    std::string line;
    std::getline(inFile, line);
    std::string before = "URL: ";
    std::string after = " Doc number:";
    size_t s = line.find(before);
    if (s == std::string::npos){
        return;
    }
    s += before.size();
    size_t e = line.find(after, s);
    if (e == std::string::npos){
        return;
    }
    std::string url = line.substr(s, e-s);
    std::cout << url << " ";
    bool in_links = false;
    while (std::getline(inFile, line)) {
        if (!line.empty() && line[0] == '<') {
            if (line.find("<links>") != std::string::npos) {
                in_links = true;
            } else if (in_links && line.find("</links>") != std::string::npos) {
                break;
            }
        } else if (in_links) {
            std::cout << line << " ";
        }
    }
    std::cout << "\n";
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Expected: " << argv[0] << "<directory_path>\n";
    }

    int count = 0;
    std::string dirpath = std::string(argv[1]);
    for (const auto &entry : fs::directory_iterator(dirpath)) {
        if (!entry.is_regular_file()) continue;
        auto path = entry.path();
        if (path.extension() == ".parsed") {
            getLinks(path);
            count++;
        }
    }
}
