#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>

std::string filename = "input/input";

struct Plot {
  char value;
  std::vector<Plot*> neighbours{};
  bool visited =  false;
  explicit Plot (const char c) : value (c) {}
  friend std::ostream& operator<<(std::ostream& os, const Plot& p) {
    os << p.value;
    return os;
  }
};

using cell = Plot*;

void visitPlots(Plot* current, size_t& area, size_t& neighbours) {
  if (current == nullptr || current->visited) return;

  current->visited = true;
  ++area;
  neighbours += current->neighbours.size();

  for (const auto p : current->neighbours) {
    visitPlots(p, area, neighbours);
  }
}

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

  for (const auto& row : grid) {
    for (auto ch : row) {
      if (!ch->visited) {
        size_t neighbours = 0;
        size_t area = 0;
        visitPlots(ch, area, neighbours);
        size_t perimeter = area * 4 - neighbours;
        result += area * perimeter;
        std::cout << *ch << ":\t" << area << "\t" << perimeter << std::endl;
      }
    }
  }

  std::cout << "Result: " << result << std::endl;
}