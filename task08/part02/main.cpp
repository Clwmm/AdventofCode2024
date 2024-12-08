#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <set>


std::string filename = "input/input";

using Grid = std::vector<std::vector<int8_t>>;

struct Position {
    int32_t x = 0;
    int32_t y = 0;

    bool operator<(const Position& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

// the maximum ASCII value that can appear in the input file
constexpr size_t MAP_SIZE = 122;

template <class K, class V>
class HashMap
{
private:
    using Pair = std::pair<K, V>;
    std::vector<V> tab[MAP_SIZE];
    std::set<K> keysOfActiveAntennas;

    static size_t hash(const K& value) {
        return value % MAP_SIZE;
    }
public:
    void add(const Pair& pair) {
        keysOfActiveAntennas.insert(pair.first);
        tab[hash(pair.first)].push_back(pair.second);
    }
    const std::vector<V>& get(const K& value) {
        return tab[hash(value)];
    }
    const std::set<K>& getAntennas() const {
        return keysOfActiveAntennas;
    }
};

bool checkBoundariesAndInsert(Grid& grid, const Position& antinode, std::set<Position>& antinodesPositions) {
    if (antinode.y >= 0 && antinode.y < (int32_t)grid.size() &&
        antinode.x >= 0 && antinode.x < (int32_t)grid[0].size()) {
        antinodesPositions.insert(antinode);
        if (grid[antinode.y][antinode.x] == '.')
            grid[antinode.y][antinode.x] = '#';
        return true;
    }
    return false;
}

int main() {
    HashMap<int8_t, Position> map;
    Grid grid;
    std::set<Position> antinodesPositions;

    std::fstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<int8_t> row;
        for (auto ch : line) {
            if (ch != '.') {
                map.add({ch, {(int32_t)row.size(), (int32_t)grid.size()}});
            }
            row.push_back(ch);
        }
        grid.push_back(row);
    }
    inputFile.close();

    auto antennas = map.getAntennas();

    for (auto antenna : antennas) {
        std::cout << antenna << ":\t";
        auto positions = map.get(antenna);
        for (size_t i = 0; i < positions.size(); ++i) {
            for (size_t j = i+1; j < positions.size(); ++j) {
                auto a = positions[i];
                auto b = positions[j];
                Position difference = {a.x - b.x, a.y - b.y};
                checkBoundariesAndInsert(grid, a, antinodesPositions);
                checkBoundariesAndInsert(grid, b, antinodesPositions);

                for (int32_t i = 1; true; ++i) {
                    Position antinode = {a.x + (difference.x * i), a.y + (difference.y * i)};
                    if (!checkBoundariesAndInsert(grid, antinode, antinodesPositions))
                        break;
                }
                for (int32_t i = 1; true; ++i) {
                    Position antinode = {b.x - (difference.x * i), b.y - (difference.y * i)};
                    if (!checkBoundariesAndInsert(grid, antinode, antinodesPositions))
                        break;
                }
            }
        }
        for (auto pos : positions) {
            std::cout << "{" << pos.x << ", " << pos.y << "} ";
        }
        std::cout << std::endl;
    }

    for (auto x : grid) {
        for (auto y : x) {
            std::cout << y;
        }
        std::cout << std::endl;
    }

    std::cout << "\n\nAntinodes:\n";
    for (auto x : antinodesPositions) {
        std::cout << "{" << x.x << ", " << x.y << "}\n";
    }
    std::cout << antinodesPositions.size() << std::endl;
}