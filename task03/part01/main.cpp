#include <regex>
#include <fstream>
#include <iostream>
#include <string>
std::string filename = "input/input";

std::regex mul_pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");

auto main () -> int {
    std::fstream inputFile(filename);
    std::string line;
    uint64_t result = 0;
    while (std::getline(inputFile, line)) {

        auto matches_begin = std::sregex_iterator(line.begin(), line.end(), mul_pattern);

        for (std::sregex_iterator it = matches_begin; it != std::sregex_iterator(); ++it) {
            std::smatch match = *it;
            uint16_t x = std::stoi(match[1].str());
            uint16_t y = std::stoi(match[2].str());
            result += (x * y);
        }
    }
    inputFile.close();

    std::cout << result << std::endl;
}