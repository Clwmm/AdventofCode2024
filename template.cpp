#include <iostream>
#include <fstream>
#include <vector>

std::string filename = "input/input";

auto main() -> int {
    std::vector<std::string> lines;

    std::fstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();
}