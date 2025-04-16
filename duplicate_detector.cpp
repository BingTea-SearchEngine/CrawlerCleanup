#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <cstdio>      
#include <algorithm>   

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

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cerr << "Expected: " << argv[0] << " [-d] <directory_path>\n";
        return 1;
    }
    bool shouldDelete = false;
    std::string dirpath;
    if (std::string(argv[1]) == "-d") {
        if (argc < 3) {
            std::cerr << "No directory after -d\n";
            return 1;
        }
        shouldDelete = true;
        dirpath = argv[2];
    } else {
        dirpath = argv[1];
    }

    std::unordered_map<std::string, std::vector<std::string>> urlToFiles;
    size_t total = 0;

    for (auto &entry : fs::directory_iterator(dirpath)) {
        auto path = entry.path();
        if (path.extension() == ".parsed") {
            std::string url = getUrl(path);
            if (!url.empty()){
                urlToFiles[url].push_back(path.filename().string());
                total++;
            }
        }
    }

    size_t numRemoved = 0;
    size_t duplicates = 0;

    for (auto &pair : urlToFiles) {
        const std::string &url = pair.first;
        std::vector<std::string> &vec = pair.second;

        std::cout << url << ":\n";

        for (auto &f : vec) {
            std::cout << "    -> " << f << "\n";
        }
        duplicates+=vec.size()-1;

        if (shouldDelete && vec.size() > 1) {
            for (size_t i = 1; i < vec.size(); i++){
                std::string fullpath = (fs::path(dirpath) / vec[i]).string();
                if (std::remove(fullpath.c_str()) == 0) {
                    numRemoved++;
                }
                else {
                    std::perror(("Error removing file " + fullpath).c_str());
                }
            }
        }
    }

    std::cout << "Number of duplicates: " << duplicates << "\n";
    if (shouldDelete) {
        std::cout << "\nOriginal document count: " << total << "\n"
                  << "Documents removed: " << numRemoved << "\n"
                  << "Final document count: " << (total - numRemoved) << "\n";
    }

    return 0;
}
