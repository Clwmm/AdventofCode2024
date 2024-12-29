#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <set>

std::string filename = "input/input";

struct Plot {
    char value;
    std::vector<Plot*> neighbours{};
    bool visited =  false;
    size_t id = 0;
    bool topEdge = true;
    bool botEdge = true;
    bool leftEdge = true;
    bool rightEdge = true;
    explicit Plot (const char c) : value (c) {}
    friend std::ostream& operator<<(std::ostream& os, const Plot& p) {
        os << p.value;
        return os;
    }
};

using cell = Plot*;

void visitPlots(Plot* current, size_t& area, size_t id) {
    if (current == nullptr || current->visited) return;

    current->visited = true;
    ++area;
    current->id = id;

    for (const auto p : current->neighbours) {
        visitPlots(p, area, id);
    }
}

struct PlotId {
    size_t id = 0;
    size_t area = 0;
    size_t sides = 0;
};

auto main() -> int {
    std::fstream inputFile(filename);
    std::string line;
    std::vector<std::vector<cell>> grid;
    while (std::getline(inputFile, line)) {
        std::vector<cell> row;
        for (size_t i = 0; i < line.size(); ++i) {
            char ch = line[i];
            auto p = new Plot(ch);
            if (!row.empty()) {
                if (row.back()->value == ch) {
                    row.back()->neighbours.push_back(p);
                    p->neighbours.push_back(row.back());
                }
            }
            if (!grid.empty()) {
                if (grid.back()[i]->value == ch) {
                    grid.back()[i]->neighbours.push_back(p);
                    p->neighbours.push_back(grid.back()[i]);
                }
            }
            row.push_back(p);
        }
        grid.push_back(row);
    }
    inputFile.close();

    size_t result = 0;
    size_t currentId = 1;

    std::vector<PlotId> plotIds;

    for (const auto& row : grid) {
        for (auto plot : row) {
            if (!plot->visited) {
                size_t area = 0;
                visitPlots(plot, area, currentId);
                plotIds.emplace_back(currentId, area, 0);
                ++currentId;
            }
        }
    }

    for (auto& plotId : plotIds) {
        std::set<size_t> set;

        // UP
        for (size_t x = 0; x < grid[0].size(); ++x) {
            bool last = false;
            for(size_t y = 0; y < grid.size(); ++y) {
                auto plot = grid[y][x];

                if (plot->id == plotId.id) {
                    if (plot->topEdge) {
                        if (!last) {
                            plot->topEdge = false;

                            for (size_t x_ = x + 1; x_ < grid[0].size(); ++x_) {
                                auto plot_ = grid[y][x_];
                                if (plot_->id == plotId.id) {
                                    if (y != 0) {
                                        auto plot__ = grid[y-1][x_];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->topEdge = false;
                                    } else plot_->topEdge = false;
                                } else break;
                            }
                            for (int x_ = (int)x - 1; x_ >= 0; --x_) {
                                auto plot_ = grid[y][x_];
                                if (plot_->id == plotId.id) {
                                    if (y != 0) {
                                        auto plot__ = grid[y-1][x_];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->topEdge = false;
                                    } else plot_->topEdge = false;
                                } else break;
                            }
                            ++plotId.sides;
                        }
                    }
                    last = true;
                }
                else last = false;
            }
        }
        // LEFT
        for (size_t y = 0; y < grid.size(); ++y) {
            bool last = false;
            for (size_t x = 0; x < grid[y].size(); ++x) {
                auto plot = grid[y][x];

                if (plot->id == plotId.id) {
                    if (plot->leftEdge) {
                        if (!last) {
                            plot->leftEdge = false;

                            for (size_t y_ = y + 1; y_ < grid.size(); ++y_) {
                                auto plot_ = grid[y_][x];
                                if (plot_->id == plotId.id) {
                                    if (x != 0) {
                                        auto plot__ = grid[y_][x - 1];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->leftEdge = false;
                                    } else plot_->leftEdge = false;
                                } else break;
                            }
                            for (int y_ = (int)y - 1; y_ >= 0; --y_) {
                                auto plot_ = grid[y_][x];
                                if (plot_->id == plotId.id) {
                                    if (x != 0) {
                                        auto plot__ = grid[y_][x - 1];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->leftEdge = false;
                                    } else plot_->leftEdge = false;
                                } else break;
                            }
                            ++plotId.sides;
                        }
                    }
                    last = true;
                }
                else last = false;
            }
        }
        // DOWN
        for (size_t x = 0; x < grid[0].size(); ++x) {
            bool last = false;
            for(int y = (int)grid.size() - 1; y >= 0; --y) {
                auto plot = grid[y][x];

                if (plot->id == plotId.id) {
                    if (plot->botEdge) {
                        if (!last) {
                            plot->botEdge = false;

                            for (size_t x_ = x + 1; x_ < grid[0].size(); ++x_) {
                                auto plot_ = grid[y][x_];
                                if (plot_->id == plotId.id) {
                                    if (y != (int)grid.size() - 1) {
                                        auto plot__ = grid[y+1][x_];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->botEdge = false;
                                    } else plot_->botEdge = false;
                                } else break;
                            }
                            for (int x_ = (int)x - 1; x_ >= 0; --x_) {
                                auto plot_ = grid[y][x_];
                                if (plot_->id == plotId.id) {
                                    if (y != (int)grid.size() - 1) {
                                        auto plot__ = grid[y+1][x_];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->botEdge = false;
                                    } else plot_->botEdge = false;
                                } else break;
                            }
                            ++plotId.sides;
                        }
                    }
                    last = true;
                }
                else last = false;
            }
        }
        // RIGHT
        for (size_t y = 0; y < grid.size(); ++y) {
            bool last = false;
            for (int x = (int)grid[y].size() - 1; x >= 0; --x) {
                auto plot = grid[y][x];

                if (plot->id == plotId.id) {
                    if (!last) {
                        set.insert(x);
                    }
                    last = true;
                }
                else last = false;
            }
        }
        // RIGHT
        for (size_t y = 0; y < grid.size(); ++y) {
            bool last = false;
            for (int x = (int)grid[y].size() - 1; x >= 0; --x) {
                auto plot = grid[y][x];

                if (plot->id == plotId.id) {
                    if (plot->rightEdge) {
                        if (!last) {
                            plot->rightEdge = false;

                            for (size_t y_ = y + 1; y_ < grid.size(); ++y_) {
                                auto plot_ = grid[y_][x];
                                if (plot_->id == plotId.id) {
                                    if (x != (int)grid[y_].size() - 1) {
                                        auto plot__ = grid[y_][x + 1];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->rightEdge = false;
                                    } else plot_->rightEdge = false;
                                } else break;
                            }
                            for (int y_ = (int)y - 1; y_ >= 0; --y_) {
                                auto plot_ = grid[y_][x];
                                if (plot_->id == plotId.id) {
                                    if (x != (int)grid[y_].size() - 1) {
                                        auto plot__ = grid[y_][x + 1];
                                        if (plot__->id == plotId.id) {
                                            break;
                                        }
                                        plot_->rightEdge = false;
                                    } else plot_->rightEdge = false;
                                } else break;
                            }
                            ++plotId.sides;
                        }
                    }
                    last = true;
                }
                else last = false;
            }
        }


        result += plotId.area * plotId.sides;
    }

    std::cout << "Result: " << result << std::endl;
}