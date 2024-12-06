#include <iostream>
#include <fstream>
#include <cstdint>
#include <set>
#include <vector>

std::string filename = "input/input";

constexpr char upChar = '^';
constexpr char obstacle = '#';
using Grid = std::vector<std::vector<uint8_t>>;

struct Position {
    size_t x = 0;
    size_t y = 0;

    bool operator<(const Position& other) const {
        return std::tie(x, y) < std::tie(other.x, other.y);
    }
};

enum Direction {
    up,
    right,
    left,
    down
};

class Guard {
    Position _pos;
    Direction dir = Direction::up;
    bool out_of_bounds = false;

public:
    void setPos(Position pos) {
        _pos = pos;
    }

    const Position getPos() const {
        return _pos;
    }

    void move(const Grid& grid, std::set<Position>& visitedTiles) {
        visitedTiles.insert(_pos);
        switch (dir) {
            case Direction::up:
                if (_pos.y == 0) {
                    out_of_bounds = true;
                    return;
                } else {
                    if (grid[_pos.y - 1][_pos.x] == obstacle) {
                        dir = Direction::right;
                    } else {
                        _pos.y--;
                    }
                }
                break;
            case Direction::right:
                if (_pos.x == grid[0].size() - 1) {
                    out_of_bounds = true;
                    return;
                } else {
                    if (grid[_pos.y][_pos.x + 1] == obstacle) {
                        dir = Direction::down;
                    } else {
                        _pos.x++;
                    }
                }
                break;
            case Direction::left:
                if (_pos.x == 0) {
                    out_of_bounds = true;
                    return;
                } else {
                    if (grid[_pos.y][_pos.x - 1] == obstacle) {
                        dir = Direction::up;
                    } else {
                        _pos.x--;
                    }
                }
                break;
            case Direction::down:
                if (_pos.y == grid.size() - 1) {
                    out_of_bounds = true;
                    return;
                } else {
                    if (grid[_pos.y + 1][_pos.x] == obstacle) {
                        dir = Direction::left;
                    } else {
                        _pos.y++;
                    }
                }
                break;
            default:
                std::cout << "Unknown direction!" << std::endl;
                break;
        }
    }

    bool isFinished() const {
        return out_of_bounds;
    }
};

auto main() -> int {
    std::set<Position> visitedTiles;
    Grid grid;
    Guard guard;

    std::fstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<uint8_t> row;
        for (auto ch : line) {
            if (ch == upChar) {
                guard.setPos({row.size(), grid.size()});
            }
            row.push_back(ch);
        }
        grid.push_back(row);
    }
    inputFile.close();

    while(!guard.isFinished())
        guard.move(grid, visitedTiles);

    for (size_t y = 0; y < grid.size(); ++y) {
        for (size_t x = 0; x < grid[0].size(); ++x) {
            if (visitedTiles.contains({x, y}))
                std::cout << "X";
            else
                std::cout << grid[y][x];
        }
        std::cout << std::endl;
    }

    std::cout << "\n" << visitedTiles.size() << std::endl;
}