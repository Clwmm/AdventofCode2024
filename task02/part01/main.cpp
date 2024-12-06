#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <cstdlib>

std::string filename = "input/input";

std::vector<std::string> getLinesFromFile(const std::string& _filename) {
    std::ifstream file(_filename);
    std::vector<std::string> lines;
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Failed to open file " << _filename << std::endl;
        exit(EXIT_FAILURE);
    }

    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    file.close();

    return lines;
}

int getRows(const std::vector<std::string>& rows) {
    int result = 0;
    for (const auto& row : rows) {

        int n = row.size() - 3;
        for (int i = 0; i < n; i++) {
            std::string p = row.substr(i, 4);
            if (p == "XMAS" || p == "SAMX") {
                result++;
            }
        }
    }
    return result;
}

std::vector<std::string> createColumns(const std::vector<std::string>& rows) {
    std::vector<std::string> columns(rows[0].size());
    for (size_t i = 0; i < rows[0].size(); ++i) {
        for (size_t j = 0; j < rows.size(); ++j) {
            columns[i] += rows[j][i];
        }
    }
    return columns;
}

int getColumns(const std::vector<std::string>& rows) {
    return getRows(createColumns(rows));
}

std::vector<std::string> createSE(const std::vector<std::string>& rows) {
    std::vector<std::string> result = rows;

    for (size_t i = 0; i < rows.size(); ++i) {
        std::string start_stars(rows.size() - (i + 1), '*');
        std::string end_stars(i, '*');
        result[i] = start_stars + result[i] + end_stars;
    }

    result = createColumns(result);

    if (result.size() > 6) {
        result.erase(result.begin(), result.begin() + 3);
        result.erase(result.end() - 3, result.end());
    }

    return result;
}

std::vector<std::string> createSW(const std::vector<std::string>& rows) {
    std::vector<std::string> result = rows;

    for (size_t i = 0; i < rows.size(); ++i) {
        std::string start_stars(rows.size() - (i + 1), '*');
        std::string end_stars(i, '*');
        result[i] = end_stars + result[i] + start_stars;
    }

    result = createColumns(result);

    if (result.size() > 6) {
        result.erase(result.begin(), result.begin() + 3);
        result.erase(result.end() - 3, result.end());
    }

    for (std::string& str : result) {
        str.erase(std::remove(str.begin(), str.end(), '*'), str.end());
    }

    return result;
}

int getSE(const std::vector<std::string>& rows) {
    return getRows(createSE(rows));
}

int getSW(const std::vector<std::string>& rows) {
    return getRows(createSW(rows));
}

auto main() -> int {
    const auto lines = getLinesFromFile(filename);

    const int rows = getRows(lines);
    const int columns = getColumns(lines);
    const int se = getSE(lines);
    const int sw = getSW(lines);
    const int sum = rows + columns + se + sw;

    std::cout << "Rows:\t\t" << rows << std::endl;
    std::cout << "Columns:\t" << columns << std::endl;
    std::cout << "SE:\t\t\t" << se << std::endl;
    std::cout << "SW:\t\t\t" << sw << std::endl;
    std::cout << "Sum:\t\t" << sum << std::endl;
}