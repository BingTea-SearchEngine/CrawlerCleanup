#include <string>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>

using std::cout, std::endl;
namespace fs = std::filesystem;

#include "Parser.hpp"

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Expected: " << argv[0] << "<html directory> <out file path> <start number>\n";
        exit(EXIT_FAILURE);
    }

    std::string dirPath = std::string(argv[1]);
    std::string outPath = std::string(argv[2]);
    int startNum = std::stoi(argv[3]);
    std::string line;

    for (const auto& entry : fs::directory_iterator(dirPath)) {
        if (!entry.is_regular_file()) continue;
        std::ifstream f(entry.path());
        std::ostringstream ss;
        ss << f.rdbuf();
        std::string html = std::move(ss.str());
        Parser parser(html);
        for (auto& w : parser.getTitle()) {
            cout << w << " ";
        }
        cout << endl;
        startNum++;
    }
}
