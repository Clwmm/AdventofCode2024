#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <queue>

std::string filename = "input/input";

auto main() -> int {
    size_t result = 0;
    std::fstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find(':') != std::string::npos) {
            size_t delimiter = line.find(':');
            std::string test_value_str = line.substr(0, delimiter);
            uint64_t test_value = stoll(test_value_str);
            std::string right = line.substr(delimiter + 2);
            std::queue<uint64_t> nums;
            std::stringstream ss(right);
            std::string num;
            while(getline(ss, num, ' ')) {
                nums.push(stoll(num));
            }

            uint64_t total_places = nums.size() - 1;
            uint64_t total_combinations = 1 << total_places;
            std::vector<std::queue<char>> combinations(total_combinations);

            for (uint64_t i = 0; i < total_combinations; ++i) {
                std::queue<char> queue;
                for (uint64_t j = 0; j < total_places; ++j) {
                    if (i & (1 << j))
                        queue.push('+');
                    else
                        queue.push('*');
                }
                combinations[i] = queue;
            }


            for (auto x : combinations) {
                std::queue<uint64_t> que = nums;
                uint64_t lastNum = que.front();
                que.pop();
                for (; !que.empty(); que.pop()) {
                    if (x.front() == '*')
                        lastNum *= que.front();
                    else
                        lastNum += que.front();
                    x.pop();
                }
                if (lastNum == test_value) {
                    result += test_value;
                    break;
                }
            }
        }
    }
    inputFile.close();

    std::cout << result << std::endl;
}