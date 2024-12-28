#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <thread>

//  Install GMP library
// sudo apt-get install libgmp-dev
#include <gmp.h>
#include <gmpxx.h>

std::string filename = "input/input";

void cleanString(std::string& s) {
    s.erase(0, s.find_first_not_of('0'));
    if (s.empty()) s = "0";
}

void processChunk(std::vector<mpz_class>& chunk) {
    for (auto it = chunk.begin(); it != chunk.end(); ++it) {
        auto& stone = *it;
        if (stone == 0)
            ++stone;
        else if (stone.get_str().size() % 2 == 0) {
            const size_t mid = stone.get_str().size() / 2;
            std::string s = stone.get_str().substr(0, mid);
            cleanString(s);
            std::string e = stone.get_str().substr(mid, mid);
            cleanString(e);
            it = chunk.erase(it);
            it = chunk.insert(it, mpz_class(s));
            it = chunk.insert(++it, mpz_class(e));
        }
        else {
            stone *= 2024;
        }
    }
}

template <class T>
std::vector<std::vector<T>> splitVector(const std::vector<T>& vec, size_t N) {
    std::vector<std::vector<T>> result;

    size_t size = vec.size();

    N = (N > size) ? size : N;

    size_t chunkSize = size / N;
    size_t remainder = size % N;

    size_t start = 0;
    for (size_t i = 0; i < N; ++i) {
        size_t end = start + chunkSize + (i < remainder ? 1 : 0);
        std::vector<T> subVec(vec.begin() + start, vec.begin() + end);
        result.push_back(subVec);
        start = end;
    }

    return result;
}

template <class T>
std::vector<T> connectVectors(const std::vector<std::vector<T>>& subVectors) {
    std::vector<T> connectedVec;

    for (const auto& subVec : subVectors) {
        connectedVec.insert(connectedVec.end(), subVec.begin(), subVec.end());
    }

    return connectedVec;
}

int main() {
    std::vector<mpz_class> stones;

    std::fstream inputFile(filename);
    std::string line;
    std::getline(inputFile, line);
    std::istringstream lineStream(line);
    std::string stone_num;
    while (lineStream >> stone_num) {
        stones.emplace_back(stone_num);
    }
    inputFile.close();

    size_t num_threads = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << num_threads << std::endl;

    size_t blinks = 33;
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < blinks; i++) {
        auto chunks = splitVector(stones, num_threads);

        threads.clear();

        for (auto& chunk : chunks) {
            threads.emplace_back(processChunk, std::ref(chunk));
        }

        for (auto& thread : threads) {
            thread.join();
        }

        stones = connectVectors(chunks);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "Result: " << stones.size() << std::endl;
    std::cout << "Time elapsed: " << duration.count() << " seconds" << std::endl;
}