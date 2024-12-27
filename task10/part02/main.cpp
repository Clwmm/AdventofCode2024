#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <optional>
#include <cmath>
#include <unordered_set>

std::string filename = "input/input";

struct Position {
    int8_t x = 0;
    int8_t y = 0;

    bool operator<(const Position& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

struct Cell {
    Position position;
    float f = 0.0f;
    float g = 0.0f;
    float h = 0.0f;
    std::vector<Cell*> neighs;
    uint8_t value;

    Cell* prev = nullptr;

    explicit Cell(const Position& position, const uint8_t& value)
        : position(position), value(value) {}

    void add_neigh(Cell* neigh) {
        this->neighs.push_back(neigh);
    }

    void clear() {
        f = 0.0f;
        g = 0.0f;
        h = 0.0f;
    }
};

class Grid {
    std::vector<std::vector<Cell*>> data;

public:
    std::vector<Cell*> trailheads;
    std::vector<Cell*> ends;
    explicit Grid(const std::string& filename) {
        std::fstream inputFile(filename);
        std::string line;
        while (std::getline(inputFile, line)) {
            std::vector<Cell*> row;
            for (auto ch : line) {
                Position position{static_cast<int8_t>(row.size()), static_cast<int8_t>(data.size())};
                auto cell = new Cell(position, ch - '0');

                if (!row.empty()) {
                    auto x = row.back();
                    if (!(x->value == '9' - '0' && cell->value == '9' - '0')) {
                        if (abs(cell->value - x->value) == 1) {
                            if (cell->value < x->value) {
                                cell->add_neigh(x);
                            }
                            else if (cell->value > x->value) {
                                x->add_neigh(cell);
                            }
                            else {
                                x->add_neigh(cell);
                                cell->add_neigh(x);
                            }
                        }
                    }

                }

                if (!data.empty()) {
                    auto y = data.back();
                    auto x = y[position.x];
                    if (!(x->value == '9' - '0' && cell->value == '9' - '0')) {
                        if (abs(cell->value - x->value) == 1) {
                            if (cell->value < x->value) {
                                cell->add_neigh(x);
                            }
                            else if (cell->value > x->value) {
                                x->add_neigh(cell);
                            }
                            else {
                                x->add_neigh(cell);
                                cell->add_neigh(x);
                            }
                        }
                    }
                }

                if (ch == '0') {
                    trailheads.push_back(cell);
                }
                if (ch == '9') {
                    for (auto neigh : cell->neighs) {
                        if (neigh->value == 9) {
                            std::cout << "True" << std::endl;
                        }
                    }
                    std::erase_if(cell->neighs, [](Cell* cell) {
                        return cell->value == 9;
                    });
                    ends.push_back(cell);
                }

                row.push_back(cell);
            }
            data.push_back(row);
        }
        inputFile.close();
    }

    void clearHeuristics() const {
        for (const auto& row : data) {
            for (const auto& cell : row) {
                cell->clear();
            }
        }
    }
};



size_t numberOfPaths(Cell* start, Cell* end, std::unordered_set<Cell*>& visited) {
    if (start == end)
        return 1;

    size_t totalPaths = 0;

    visited.insert(start);

    for (Cell* neighbor : start->neighs)
        if (visited.find(neighbor) == visited.end())
            totalPaths += numberOfPaths(neighbor, end, visited);

    visited.erase(start);

    return totalPaths;
}

size_t numberOfPaths(Cell* start, Cell* end) {
    std::unordered_set<Cell*> visited;
    return numberOfPaths(start, end, visited);
}

int main() {
    Grid grid(filename);

    size_t result = 0;

    for (const auto& start : grid.trailheads) {
        for (const auto& end : grid.ends) {
            result += numberOfPaths(start, end);
        }
    }

    std::cout << result << '\n';
}