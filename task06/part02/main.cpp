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

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
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
    std::set<std::pair<Position, Direction>> visitedTilesWithDirection;
    std::set<Position> visitedTiles;

    void addTile()
    {
        visitedTiles.insert(_pos);
    }

    void addTileWithLoopCheck()
    {
        visitedTilesWithDirection.insert({_pos, dir});
    }

    bool checkLoop()
    {
        if (visitedTilesWithDirection.contains({_pos, dir}))
            return true;
        return false;
    }

public:

    void clearMemory(Position pos) {
        _pos = pos;
        dir = Direction::up;
        out_of_bounds = false;
        visitedTilesWithDirection.clear();
        visitedTiles.clear();
    }

    void setPos(Position pos) {
        _pos = pos;
    }

    const Position getPos() const {
        return _pos;
    }

    const std::set<Position>& getVisitedTiles() const {
        return visitedTiles;
    }

    void move(const Grid& grid) {
        addTile();
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

    bool moveWithCheckLoop(const Grid& grid) {
        if (checkLoop())
            return true;
        addTileWithLoopCheck();
        switch (dir) {
            case Direction::up:
                if (_pos.y == 0) {
                    out_of_bounds = true;
                    return false;
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
                    return false;
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
                    return false;
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
                    return false;
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
        return false;
    }

    bool isFinished() const {
        return out_of_bounds;
    }
};

auto main() -> int {
    Grid grid;
    Guard guard;
    Position startPos;

    std::fstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        std::vector<uint8_t> row;
        for (auto ch : line) {
            if (ch == upChar) {
                guard.setPos({row.size(), grid.size()});
                startPos = guard.getPos();
            }
            row.push_back(ch);
        }
        grid.push_back(row);
    }
    inputFile.close();

    // Discovering all possible locations
    while(!guard.isFinished())
        guard.move(grid);
    auto visitedTiles = guard.getVisitedTiles();


    size_t num_of_loops = 0;
    for (auto tile: visitedTiles) {
        if (tile == startPos)
            continue;
        Grid gridWithAdditionalObstacle = grid;
        gridWithAdditionalObstacle[tile.y][tile.x] = obstacle;
        guard.clearMemory(startPos);

        while(!guard.isFinished()) {
            if (guard.moveWithCheckLoop(gridWithAdditionalObstacle)) {
                num_of_loops++;
                break;
            }
        }
    }

    std::cout << "\n" << num_of_loops << std::endl;
}