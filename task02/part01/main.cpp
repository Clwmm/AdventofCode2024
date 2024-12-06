#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <cstdint>
#include <optional>

std::string filename = "input/input";
using type = int8_t;

std::optional<type> getDifference(const type& a, const type& b) {
    type result = a - b;
    if (abs(result) > 3 || result == 0)
        return std::nullopt;
    return result;
}

bool checkMonotonicity(const type& a, const type& b) {
    if ((a > 0 && b > 0) || (a < 0 && b < 0))
        return true;
    return false;
}

int main() {
    std::fstream inputFile(filename);
    std::string line;
    size_t noSafeReports = 0;
    while (std::getline(inputFile, line)) {
        std::vector<type> nums;
        std::stringstream ss(line);
        std::string num;
        while(getline(ss, num, ' ')) {
            nums.push_back((type)(stoi(num)));
        }

        std::optional<type> reference = getDifference(nums[0], nums[1]);
        if (!reference)
            continue;

        bool safe = true;
        for (size_t i = 2; i < nums.size(); ++i) {
            std::optional<type> diff = getDifference(nums[i - 1], nums[i]);
            if (!diff) {
                safe = false;
                break;
            }
            if (!checkMonotonicity(*reference, *diff))
                safe = false;
        }
        if (safe) {
            noSafeReports++;
        }

    }
    inputFile.close();

    std::cout << noSafeReports << std::endl;
}