#include <immintrin.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::string filename = "input/input";

template <class T>
void insertionSort(std::vector<T>& v)
{
    T temp;
    for (int j = v.size() - 2; j >= 0; j--)
    {
        temp = v[j];
        int i = j + 1;
        while ((static_cast<size_t>(i) < v.size()) && (temp > v[i]))
        {
            v[i - 1] = v[i];
            i++;
        }
        v[i - 1] = temp;
    }
}

int main() {

    std::fstream inputFile(filename);
    std::vector<int32_t> leftVec;
    std::vector<int32_t> rightVec;

    std::string line;
    while (std::getline(inputFile, line)) {
        std::stringstream ss(line);
        int x, y;
        ss >> x >> y;
        leftVec.push_back((int32_t)x);
        rightVec.push_back((int32_t)y);
    }
    inputFile.close();
    insertionSort(leftVec);
    insertionSort(rightVec);

    if (leftVec.size() % 8 != 0) {
        while (leftVec.size() % 8 != 0) {
            leftVec.push_back(0);
            rightVec.push_back(0);
        }
    }

    int32_t result = 0;
    for (size_t i = 0; i < leftVec.size(); i += 8) {
        __m256i left_vec = _mm256_set_epi32(leftVec[i], leftVec[i+1], leftVec[i+2], leftVec[i+3], leftVec[i+4], leftVec[i+5], leftVec[i+6], leftVec[i+7]);
        __m256i right_vec = _mm256_set_epi32(rightVec[i], rightVec[i+1], rightVec[i+2], rightVec[i+3], rightVec[i+4], rightVec[i+5], rightVec[i+6], rightVec[i+7]);

        // subtract
        __m256i result_vec = _mm256_sub_epi32(left_vec, right_vec);

        // absolute value
        result_vec = _mm256_abs_epi32(result_vec);

        // sum all elements
        __m128i sum_low = _mm256_castsi256_si128(result_vec);
        __m128i sum_high = _mm256_extracti128_si256(result_vec, 1);
        __m128i sum = _mm_add_epi32(sum_low, sum_high);
        sum = _mm_hadd_epi32(sum, sum);
        sum = _mm_hadd_epi32(sum, sum);

        // get result
        result += _mm_extract_epi32(sum, 0);
    }

    std::cout << "Result: " << result << std::endl;

    return 0;
}