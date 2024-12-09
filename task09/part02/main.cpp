#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>
#include <set>

std::string filename = "input/input";

struct MemoryBlock {
    size_t startIdx;
    size_t size;

    MemoryBlock(size_t startIdx, size_t size)
            : startIdx(startIdx), size(size) {}
};

struct FileBlock : public MemoryBlock {
    size_t id;

    FileBlock(size_t startIdx, size_t size, size_t id)
            : MemoryBlock(startIdx, size), id(id) {}
};

int main() {
    std::vector<FileBlock> files;
    std::vector<MemoryBlock> freeSpaces;
    std::set<std::pair<size_t, size_t>> set;
    std::set<std::pair<size_t, int64_t>> fullSet;

    std::fstream inputFile(filename);
    std::string line;
    size_t actualId = 0;
    size_t lastFreeIdx = 0;
    bool memoryBlock = true;
    while (std::getline(inputFile, line)) {
        for (const auto& ch : line) {
            int64_t block_size = ch - '0';
            if (block_size == 0) {
                memoryBlock ^= true;
                continue;
            }

            if (memoryBlock) {
                files.emplace_back(lastFreeIdx, block_size, actualId);
                actualId++;
            }
            else
                freeSpaces.emplace_back(lastFreeIdx, block_size);
            memoryBlock ^= true;
            lastFreeIdx = lastFreeIdx + block_size;
        }
    }
    inputFile.close();

    for (const auto x : files) {
        for (size_t i = x.startIdx; i < x.startIdx + x.size; ++i) {
            fullSet.emplace(i, (int64_t)x.id);
        }
    }

    for (const auto x : freeSpaces) {
        for (size_t i = x.startIdx; i < x.startIdx + x.size; ++i) {
            fullSet.emplace(i, -1);
        }
    }

    for (const auto& x : fullSet) {
        if (x.second == -1)
            std::cout << ".";
        else
            std::cout << x.second;
    }
    std::cout << std::endl;

//    while (true) {
//        auto& freeSpace = freeSpaces[0];
//        auto& file = files[files.size() - 1];
//        if (freeSpace.startIdx + freeSpace.size > file.startIdx)
//            break;
//        const auto id = file.id;
//        movedFiles.emplace_back(freeSpace.startIdx, 1, id);
//        file.size--;
//        if (file.size == 0)
//            files.erase(files.end());
//
//        freeSpace.startIdx++;
//        freeSpace.size--;
//        if (freeSpace.size == 0)
//            freeSpaces.erase(freeSpaces.begin());
//    }

    for (int64_t i = (int64_t)files.size() - 1; i >= 0; --i) {
        auto& file = files[i];

        for (auto it = freeSpaces.begin(); it != freeSpaces.end();) {
            auto& freeSpace = *it;

            if (freeSpace.size < file.size) {
                ++it;
                continue;
            }

            file.startIdx = freeSpace.startIdx;
            freeSpace.size -= file.size;
            freeSpace.startIdx += file.size;

            if (freeSpace.size == 0) {
                it = freeSpaces.erase(it);
            } else {
                ++it;
            }
            break;
        }

    }

    int64_t result = 0;

    for (const auto x : files) {
        for (size_t i = x.startIdx; i < x.startIdx + x.size; ++i) {
            result += (int64_t)(i * x.id);
            set.emplace(i, x.id);
        }
    }

    for (const auto& x : set) {
        std::cout << x.first << " | " << x.second << std::endl;
    }

    std::cout << result << std::endl;
}

// 15834516286076 too high
// 8564936405055 too high