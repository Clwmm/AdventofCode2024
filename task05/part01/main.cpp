#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <cstdint>

std::string filename = "input/input";

// input values between: 10-99
constexpr size_t MAP_SIZE = 90;

template <class K, class V>
class HashMap
{
private:
    using Pair = std::pair<K, V>;
    std::vector<V> tab[MAP_SIZE];

    static size_t hash(const K& value) {
        return value % MAP_SIZE;
    }
public:
    void add(const Pair& pair) {
        tab[hash(pair.first)].push_back(pair.second);
    }
    const std::vector<V>& get(const K& value) {
        return tab[hash(value)];
    }
};

template<class T>
bool isInVector(const std::vector<T>& vec, const T& value) {
    for (auto &x: vec) {
        if (x == value)
            return true;
    }
    return false;
}

int main() {
    using type = uint8_t;
    HashMap<type, type> map;
    std::vector<std::vector<type>> updateLines;
    uint64_t result = 0;

    std::fstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find('|') != std::string::npos) {
            size_t delimiter = line.find('|');
            type left = (type)(stoi(line.substr(0, delimiter)));
            type right = (type)(stoi(line.substr(delimiter + 1)));
            map.add({left, right});
        } else if (line.find(',') != std::string::npos) {
            std::vector<type> nums;
            std::stringstream ss(line);
            std::string num;
            while(getline(ss, num, ',')) {
                nums.push_back((type)(stoi(num)));
            }
            updateLines.push_back(nums);

            type middleNumber = nums[nums.size() / 2];
            bool badLine = false;
            for (int i = (int)nums.size() - 1; i >= 0; --i) {
                for (int j = i - 1; j >= 0; --j) {
                    auto vec = map.get(nums[i]);
                    if (isInVector(vec, nums[j])) {
                        badLine = true;
                        break;
                    }
                }
                if (badLine)
                    break;
            }
            if (!badLine) {
                result += middleNumber;
            }
        }
    }
    inputFile.close();

    std::cout << result << std::endl;
}