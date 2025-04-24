#include <cstdlib>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>

using std::cout, std::endl;
int main(int argc, char** argv) {
    if (argc < 3) {
        cout << "Input: <file path> <offset>";
        exit(EXIT_FAILURE);
    }

    std::string document_path = std::string(argv[1]);
    int targetOffset = std::atoi(argv[2]);
    std::string target;

    std::ifstream document(document_path);
    if (!document) {
        std::cerr << "Error opening file " << document_path << endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    std::getline(document, line); //get url and doc

    // Check <title> tag
    std::getline(document, line);

    cout << "======= TITLE ======" << endl;

    uint32_t offset = 0;
    // Get title words
    std::getline(document, line);
    std::istringstream titleIss(line);
    std::string word;
    uint32_t numTitleWords = 0;
    while (titleIss >> word) {
        cout << offset << ": " << word << endl;
        if (offset == targetOffset) {
            target = word;
        }
        ++offset;
    }

    // Check </title> tag
    std::getline(document, line);
    // Check <words>  tag
    std::getline(document, line);
    cout << "======= WORDS ======" << endl;

    std::getline(document, line);
    std::istringstream bodyIss(line);
    while (bodyIss >> word) {
        cout << offset << ": " << word << endl;
        if (offset == targetOffset) {
            target = word;
        }
        ++offset;
    }
    cout << "TARGET: " << target << endl;
}
