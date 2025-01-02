#include <iostream>
#include <fstream>
#include <regex>

std::string filename = "input/input";

auto main() -> int {
    std::fstream inputFile(filename);
    std::string line;

    std::regex buttonRegex("Button");

    while (std::getline(inputFile, line)) {
        std::cout << line << std::endl;
    }
    inputFile.close();
}