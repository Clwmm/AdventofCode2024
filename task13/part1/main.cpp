#include <iostream>
#include <fstream>
#include <vector>
#include <regex>

std::string filename = "input/input";

struct Button {
    size_t x;
    size_t y;
};

struct Machine {
    Button buttonA;
    Button buttonB;
    Button prize;
};

auto main() -> int {
    std::vector<Machine> machines;
    std::fstream inputFile(filename);
    std::string line;

    std::regex buttonPattern(R"((Button [A-Z]:).*X\+(\d+), Y\+(\d+))");
    std::regex prizePattern(R"(Prize:.*X=(\d+), Y=(\d+))");
    std::smatch match;

    while (std::getline(inputFile, line)) {
        if (std::regex_search(line, match, buttonPattern)) {
            Machine m{};
            m.buttonA.x = std::stoi(match[2]);
            m.buttonA.y = std::stoi(match[3]);

            std::getline(inputFile, line);
            std::regex_search(line, match, buttonPattern);
            m.buttonB.x = std::stoi(match[2]);
            m.buttonB.y = std::stoi(match[3]);

            std::getline(inputFile, line);
            std::regex_search(line, match, prizePattern);
            m.prize.x = std::stoi(match[1]);
            m.prize.y = std::stoi(match[2]);

            machines.push_back(m);
        }
    }
    inputFile.close();

    for (const auto& m : machines) {
        std::cout << "Button A: X+" << m.buttonA.x << ", Y+" << m.buttonA.y << std::endl;
        std::cout << "Button B: X+" << m.buttonB.x << ", Y+" << m.buttonB.y << std::endl;
        std::cout << "Prize: X=" << m.prize.x << ", Y=" << m.prize.y << std::endl << std::endl;
    }
}