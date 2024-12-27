#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <fstream>
#include <optional>
#include <cmath>

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
                    ends.push_back(cell);
                }

                row.push_back(cell);
            }
            data.push_back(row);
        }
        inputFile.close();
    }

    ~Grid() {
        for (const auto& row : data) {
            for (const auto cell : row) {
                delete cell;
            }
        }
    }

    void clearHeuristics() const {
        for (const auto& row : data) {
            for (const auto& cell : row) {
                cell->clear();
            }
        }
    }
};

void removeFromSet(std::vector<Cell*>& set, const Cell* cell) {
    if (const auto it = std::ranges::find(set, cell); it != set.end())
        set.erase(it);
}

bool isInSet(std::vector<Cell*>& set, const Cell* cell) {
    if (const auto it = std::ranges::find(set, cell); it != set.end())
        return true;
    return false;
}

float heuristics(const Cell* neighbor, const Cell* end)
{
    const auto x_dif = static_cast<float>(std::abs(neighbor->position.x - end->position.x));
    const auto y_dif = static_cast<float>(std::abs(neighbor->position.y - end->position.y));

    return std::sqrt(std::pow(x_dif, 2) + std::pow(y_dif, 2));
}

bool checkTrail(Cell* start, const Cell* end)
{
    std::vector<Cell*> openSet;
    openSet.push_back(start);

    std::vector<Cell*> closedSet;

    std::vector<Cell*> path;

    while (!openSet.empty()) {
        size_t winner = 0;
        for (size_t i = 0; i < openSet.size(); ++i) {
            if (openSet[i]->f < openSet[winner]->f) {
                winner = i;
            }
        }
        auto current = openSet[winner];
        if (current == end) {
            return true;
        }

        removeFromSet(openSet, current);
        closedSet.push_back(current);

        const auto neighs = current->neighs;
        for (const auto& x : neighs) {
            if (!isInSet(closedSet, x)) {
                const auto tempG = current->g + 1.f;

                if (isInSet(openSet, x)) {
                    if (tempG < x->g) {
                        x->g = tempG;
                    }
                }
                else {
                    x->g = tempG;
                    openSet.push_back(x);
                }

                x->h = heuristics(x, end);
                x->f = x->g + x->h;
                x->prev = current;
            }
        }
    }

    return false;
}

int main() {
    Grid grid(filename);
    size_t result = 0;

    for (const auto& start : grid.trailheads) {
        for (const auto& end : grid.ends) {
            if (checkTrail(start, end))
                ++result;
            grid.clearHeuristics();
        }
    }

    std::cout << result << '\n';
}