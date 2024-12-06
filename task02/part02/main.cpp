#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cstdlib>

std::string filename = "input/input";

std::vector<std::string> getLinesFromFile(const std::string& _filename) {
    std::ifstream file(_filename);
    std::vector<std::string> lines;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << _filename << std::endl;
        exit(EXIT_FAILURE);
    }

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return lines;
}

bool checkX(const std::string& first, const std::string& second) {
    if ((first == "MAS" || first == "SAM") &&
        (second == "MAS" || second == "SAM")) {
        return true;
    }
    return false;
}

int run(const std::vector<std::string>& rows) {
    int result = 0;
    std::vector<std::vector<char>> charVec;

    for (const std::string& str : rows) {
        std::vector<char> charVector(str.begin(), str.end());
        charVec.push_back(charVector);
    }

    for (size_t i = 1; i < charVec.size() - 1; ++i) {
        for (size_t j = 1; j < charVec[0].size() - 1; ++j) {
            std::string first = {rows[i-1][j-1], rows[i][j], rows[i+1][j+1]};
            std::string second = {rows[i-1][j+1], rows[i][j], rows[i+1][j-1]};

            if (checkX(first, second)) {
                result++;
            }

            // std::cout << rows[i-1][j-1] << "*" << rows[i-1][j+1] << std::endl;
            // std::cout << "*" << rows[i][j] << "*" << std::endl;
            // std::cout << rows[i+1][j-1] << "*" << rows[i+1][j+1] << std::endl;
        }
    }
    return result;
}

auto main() -> int {
    const auto lines = getLinesFromFile(filename);
    std::cout << "Sum:\t" << run(lines) << std::endl;
}