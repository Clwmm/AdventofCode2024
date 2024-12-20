#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <vector>

std::string filename = "input/input";

struct MemoryBlock {
    size_t startIdx;
    size_t size;
    bool active = true;

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


    for (int64_t i = static_cast<int64_t>(files.size()) - 1; i >= 0; --i) {
        auto& file = files[i];

        for (auto &freeSpace : freeSpaces) {
            if (freeSpace.size < file.size || !freeSpace.active) {
                continue;
            }
            if (file.startIdx < freeSpace.startIdx) {
                break;
            }

            file.startIdx = freeSpace.startIdx;
            freeSpace.size -= file.size;
            freeSpace.startIdx += file.size;

            if (freeSpace.size == 0) {
                freeSpace.active = false;
            }
            break;
        }
    }

    int64_t result = 0;

    for (const auto x : files) {
        for (size_t i = x.startIdx; i < x.startIdx + x.size; ++i) {
            result += (int64_t)(i * x.id);
        }
    }

    std::cout << result << std::endl;
}