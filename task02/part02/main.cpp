#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cstdint>
#include <optional>

std::string filename = "input/input";
using type = int8_t;
using option = std::optional<type>;

option getDifference(const type& a, const type& b) {
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

bool checkNums(const std::vector<type>& nums) {
    option ref = getDifference(nums[0], nums[1]);
    if (!ref)
        return false;
    for (size_t i = 2; i < nums.size(); ++i) {
        option diff = getDifference(nums[i -1], nums[i]);
        if (!diff)
            return false;
        if (!checkMonotonicity(*ref, *diff))
            return false;
    }
    return true;
}

class Nums {
    std::vector<type> nums;
    bool safe = false;
public:
    Nums(std::vector<type>& nums)
        : nums(nums) {
        safe = checkNums(nums);
    };

    [[nodiscard]] bool areSafe() const {
        return safe;
    }

    bool removeAndCheckAll() {
        for (size_t i = 0; i < nums.size(); ++i) {
            std::vector<type> vec = nums;
            vec.erase(vec.begin() + i);
            if (checkNums(vec))
                return true;
        }
        return false;
    }


};

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
        Nums nums1(nums);
        if (nums1.areSafe())
            noSafeReports++;
        else
            if (nums1.removeAndCheckAll())
                noSafeReports++;
    }
    inputFile.close();

    std::cout << noSafeReports << std::endl;
}