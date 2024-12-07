#include <iostream>
#include <fstream>
#include <cstdint>
#include <set>
#include <vector>
#include <chrono>
#include <future>
#include <tuple>

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

size_t processSet(std::set<Position>::iterator start, std::set<Position>::iterator end, const Position& startPos, const Grid& grid) {
    size_t num_of_loops = 0;
    Guard guard;
    guard.setPos(startPos);
    for (auto it = start; it != end; ++it) {
        auto tile = *it;
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
    return num_of_loops;
}

std::vector<std::pair<std::set<Position>::iterator, std::set<Position>::iterator>> splitSet(
        std::set<Position>& input_set, size_t num_parts) {

    std::vector<std::pair<std::set<Position>::iterator, std::set<Position>::iterator>> result;
    auto start_it = input_set.begin();
    //auto end_it = input_set.end();

    size_t total_size = input_set.size();
    size_t part_size = total_size / num_parts;
    size_t remainder = total_size % num_parts;

    // Split the set into roughly equal parts
    for (size_t i = 0; i < num_parts; ++i) {
        auto end_part_it = start_it;
        for (size_t j = 0; j < part_size + (i < remainder ? 1 : 0); ++j) {
            ++end_part_it;
        }
        result.push_back({start_it, end_part_it});
        start_it = end_part_it;
    }

    return result;
}

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

    auto start = std::chrono::high_resolution_clock::now();

    // Discovering all possible locations
    while(!guard.isFinished())
        guard.move(grid);
    auto visitedTiles = guard.getVisitedTiles();
    std::cout << "Visited tiles: " << visitedTiles.size() << std::endl;

    size_t num_threads = 7;
    auto split_parts = splitSet(visitedTiles, num_threads);

    std::vector<std::future<size_t>> futures;

    for (size_t i = 0; i < num_threads; ++i) {
        // Launch each thread asynchronously and store the future
        futures.push_back(std::async(std::launch::async, processSet, split_parts[i].first, split_parts[i].second, startPos, grid));
    }

    size_t num_of_loops = 0;
    for (size_t i = 0; i < num_threads; ++i) {
        size_t result = futures[i].get();
        num_of_loops += result;
        std::cout << "Result from Thread " << i << ": " << result << std::endl;
    }


    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::cout << "\n" << num_of_loops << std::endl;
    std::cout << "Time elapsed: " << duration.count() << " seconds" << std::endl;
}