#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <queue>

//  Install GMP library
// sudo apt-get install libgmp-dev
#include <gmp.h>
#include <gmpxx.h>

std::string filename = "input/input";

auto main() -> int {
    mpz_class result("0");
    std::fstream inputFile(filename);
    std::string line;
    float nr_line = 1;
    float max_liens = 850;
    while (std::getline(inputFile, line)) {
        if (line.find(':') != std::string::npos) {
            std::cout << (int)(nr_line / max_liens * 100) << "%\n";
            nr_line++;

            size_t delimiter = line.find(':');
            mpz_class test_value(line.substr(0, delimiter));
            std::string right = line.substr(delimiter + 2);
            std::queue<mpz_class> nums;
            std::stringstream ss(right);
            std::string num;
            while(getline(ss, num, ' ')) {
                nums.emplace(num);
            }

            uint64_t total_places = nums.size() - 1;
            uint64_t total_combinations = 1;
            for (uint64_t i = 0; i < total_places; ++i) {
                total_combinations *= 3;
            }
            std::vector<std::queue<char>> combinations(total_combinations);

            for (uint64_t i = 0; i < total_combinations; ++i) {
                std::queue<char> queue;
                uint64_t current = i;
                for (uint64_t j = 0; j < total_places; ++j) {
                    uint64_t remainder = current % 3;
                    current /= 3;

                    switch (remainder) {
                        case 0:
                            queue.push('+');
                            break;
                        case 1:
                            queue.push('*');
                            break;
                        default:
                            queue.push('|');
                            break;
                    }
                }
                combinations[i] = queue;
            }


            for (auto x : combinations) {
                std::queue<mpz_class> que = nums;
                mpz_class lastNum = que.front();
                que.pop();
                for (; !que.empty(); que.pop()) {
                    switch (x.front()) {
                        case '+':
                            lastNum += que.front();
                            break;
                        case '*':
                            lastNum *= que.front();
                            break;
                        default:
                            std::string concatenation = lastNum.get_str();
                            concatenation += que.front().get_str();
                            lastNum = mpz_class(concatenation);
                            break;
                    }
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

// 79622678830 too low