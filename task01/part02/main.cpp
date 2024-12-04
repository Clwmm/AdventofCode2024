#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::string filename = "input/input";

constexpr size_t MAP_SIZE = 90000;

class HashMap
{
private:
    int32_t tab[MAP_SIZE]{};

    static size_t hash(const int32_t value) {
        return value % MAP_SIZE;
    }
public:
    void add(const int32_t value) {
        tab[hash(value)]++;
    }
    int32_t get(const int32_t value) {
        return tab[hash(value)];
    }
};

int main() {

    std::fstream inputFile(filename);
    std::vector<int32_t> leftVec;
    std::vector<int32_t> rightVec;
    int64_t result = 0;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        int x, y;
        ss >> x >> y;
        leftVec.push_back((int32_t)x);
        rightVec.push_back((int32_t)y);
    }
    inputFile.close();

    HashMap rightMap;

    for (size_t i = 0; i < rightVec.size(); ++i) {
        rightMap.add(rightVec[i]);
    }


    for (auto x: leftVec) {
        result += x * rightMap.get(x);
    }


    std::cout << "Result: " << result << std::endl;

    return 0;
}