#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <queue>
#include <chrono>
#include <future>

//  Install GMP library
#include <gmp.h>
#include <gmpxx.h>

std::string filename = "input/input";

mpz_class processLine(const std::vector<std::string>& lines) {
    mpz_class result;
    for (const auto& line : lines) {
        if (line.find(':') != std::string::npos) {
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
    return result;
}

std::vector<std::vector<std::string>> splitLines(std::vector<std::string>& lines, size_t numThreads) {
    size_t linesSize = lines.size();
    size_t chunkSize = (linesSize + numThreads - 1) / numThreads;

    std::vector<std::vector<std::string>> result;

    for (size_t i = 0; i < numThreads; ++i) {
        const size_t start = i * chunkSize;
        const size_t end = std::min((start + chunkSize), linesSize);

        std::vector<std::string> chunk(lines.begin() + start, lines.begin() + end);
        result.push_back(chunk);
    }

    return result;
}

auto main() -> int {
    mpz_class result("0");
    std::fstream inputFile(filename);
    std::string line;
    std::vector<std::string> lines;
    const size_t numThreads = std::thread::hardware_concurrency();

    auto start = std::chrono::high_resolution_clock::now();

    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }
    inputFile.close();

    std::vector<std::future<mpz_class>> futures;

    auto chunks = splitLines(lines, numThreads);

    for (const auto& chunk : chunks) {
        futures.push_back(std::async(std::launch::async, processLine, chunk));
    }

    for (size_t i = 0; i < numThreads; ++i) {
        auto _result = futures[i].get();
        result += _result;
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << result << std::endl;
    std::cout << "Time elapsed: " << duration.count() << " seconds" << std::endl;
}