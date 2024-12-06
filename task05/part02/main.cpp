#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <utility>
#include <cstdint>
#include <stack>

std::string filename = "input/input";

template<class T>
bool isInVector(const std::vector<T>& vec, const T& value) {
    for (auto &x: vec) {
        if (x == value)
            return true;
    }
    return false;
}

// input values between: 10-99
constexpr size_t MAP_SIZE = 90;

template <class K, class V>
class HashMap
{
private:
    using Pair = std::pair<K, V>;
    std::vector<V> tab[MAP_SIZE];

    static size_t hash(const K& value) {
        return value % MAP_SIZE;
    }
public:
    void add(const Pair& pair) {
        tab[hash(pair.first)].push_back(pair.second);
    }
    [[nodiscard]] const std::vector<V>& get(const K& value) const {
        return tab[hash(value)];
    }
};

template<class T>
struct BinaryNode
{
    T value;
    BinaryNode* right;
    BinaryNode* left;
    explicit BinaryNode(const T& value)
        : value(value), right(nullptr), left(nullptr) {}
};

template<class T>
class BinaryTree {
    using node = BinaryNode<T>*;
    node root = nullptr;
    std::vector<T> inorderVec;

    void insert_(node& node, const T x, const HashMap<T, T>& map) {
        if (node == nullptr) {
            node = new BinaryNode(x);
            return;
        }

        const auto& vec = map.get(x);
        if (isInVector(vec, node->value)) {
            this->insert_(node->left, x, map);
        }
        else {
            this->insert_(node->right, x, map);
        }
    }

    void inorder_(node node)
    {
        if (node != nullptr)
        {
            this->inorder_(node->left);
            inorderVec.push_back(node->value);
            this->inorder_(node->right);
        }
    }

    void clear_(node& node) {
        if (node != nullptr) {
            clear_(node->left);
            clear_(node->right);
            delete node;
            node = nullptr;
        }
    }

public:

    [[nodiscard]] T getMiddleElement() const {
        int middleElement = inorderVec[inorderVec.size() / 2];
        return (T)middleElement;
    }

    void insert(const T& x, const HashMap<T, T>& map) {
        if (root == nullptr) {
            root = new BinaryNode(x);
            return;
        }

        const auto& vec = map.get(x);
        if (isInVector(vec, root->value)) {
            this->insert_(root->left, x, map);
        }
        else {
            this->insert_(root->right, x, map);
        }

    }

    void clear() {
        clear_(root);
    }

    void inorder()
    {
        inorderVec.clear();
        this->inorder_(root);
    }
};

int main() {
    using type = uint8_t;
    BinaryTree<type> bt;
    HashMap<type, type> map;
    std::vector<std::vector<type>> updateLines;

    std::fstream inputFile(filename);
    std::string line;
    while (std::getline(inputFile, line)) {
        if (line.find('|') != std::string::npos) {
            size_t delimiter = line.find('|');
            type left = (type)(stoi(line.substr(0, delimiter)));
            type right = (type)(stoi(line.substr(delimiter + 1)));
            map.add({left, right});
        } else if (line.find(',') != std::string::npos) {
            std::vector<type> nums;
            std::stringstream ss(line);
            std::string num;
            while(getline(ss, num, ',')) {
                nums.push_back((type)(stoi(num)));
            }
            updateLines.push_back(nums);
        }
    }
    inputFile.close();

    uint64_t result = 0;
    for (auto _line : updateLines) {
        bool badLine = false;
        for (int i = (int)_line.size() - 1; i >= 0; --i) {
            for (int j = i - 1; j >= 0; --j) {
                const auto& vec = map.get(_line[i]);
                if (isInVector(vec, _line[j])) {
                    badLine = true;
                    break;
                }
            }
            if (badLine)
                break;
        }
        if (badLine) {
            for (auto x : _line) {
                bt.insert(x, map);
            }
            bt.inorder();
            result += bt.getMiddleElement();
            bt.clear();
        }
    }

    std::cout << result << std::endl;
}