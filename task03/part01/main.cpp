#include <regex>
#include <fstream>
#include <iostream>
#include <string>
std::string filename = "input/input";

std::regex mul_pattern(R"(mul\((\d{1,3}),(\d{1,3})\))");
std::regex do_pattern(R"(do\(\))");
std::regex dont_pattern(R"(don't\(\))");

auto main () -> int {
    std::fstream inputFile(filename);
    std::string line;
    uint64_t result = 0;
    std::string lastOp = "do";
    while (std::getline(inputFile, line)) {

        while (!line.empty()) {
            // Find the first occurrence of each pattern
            std::smatch mul_match, do_match, dont_match;
            bool found_mul = std::regex_search(line, mul_match, mul_pattern);
            bool found_do = std::regex_search(line, do_match, do_pattern);
            bool found_dont = std::regex_search(line, dont_match, dont_pattern);

            size_t mul_pos = found_mul ? mul_match.position() : std::string::npos;
            size_t do_pos = found_do ? do_match.position() : std::string::npos;
            size_t dont_pos = found_dont ? dont_match.position() : std::string::npos;
            size_t pos_to_erase;
            size_t match_lenght;


            if (mul_pos < do_pos && mul_pos < dont_pos) {
                if (lastOp == "do") {
                    uint16_t x = std::stoi(mul_match[1].str());
                    uint16_t y = std::stoi(mul_match[2].str());
                    result += (x * y);
                }
                pos_to_erase = mul_pos;
                match_lenght = mul_match.length();
            } else if (do_pos < mul_pos && do_pos < dont_pos) {
                pos_to_erase = do_pos;
                match_lenght = do_match.length();
            } else if (dont_pos < mul_pos && dont_pos < do_pos) {
                pos_to_erase = dont_pos;
                match_lenght = dont_match.length();
            } else {
                break;
            }

            line = line.substr(pos_to_erase + match_lenght);
        }
    }
    inputFile.close();

    std::cout << result << std::endl;
}


// 83596387 to high