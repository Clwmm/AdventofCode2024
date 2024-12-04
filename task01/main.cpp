#include <iostream>

constexpr int test(const int& a) {
    return a*2;
}

auto main() -> int {
    constexpr int b = test(2);
    return b;
}