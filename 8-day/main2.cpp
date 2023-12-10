#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <stdint.h>

typedef std::string string;

struct Node {
    string left;
    string right;
};

enum Dir { Left, Right };

int main() {
    using std::ranges::fold_left;
    using std::ranges::for_each;

    std::ifstream input("../input.txt");

    string instructions_str; input >> instructions_str;

    std::vector<Dir> instructions;
    for (char c : instructions_str) { instructions.push_back(c == 'L' ? Dir::Left : Dir::Right); }

    std::unordered_map<string, Node> nodes;
    std::vector<Node> current;

    string node_name;
    while (input >> node_name) {
        string eqs; string left; string right;
        input >> eqs;
        input >> left;
        input >> right;
        //std::cout << node_name << " " << left << " " << right << std::endl;
        nodes[node_name] = { left.substr(1, left.size()-2), right.substr(0, right.size()-1) };
        if (*node_name.rbegin() == 'A') { current.push_back(nodes[node_name]); }
    }

    //for (auto n : nodes) {
    //    std::cout << n.first << " " << n.second.left << " " << n.second.right << std::endl;
    //}

    uint32_t sum = 0;

    std::vector<int> found(current.size(), false);
    std::vector<std::pair<uint32_t, uint32_t>> path_size(current.size(), {0, 0});
    Node c = current[0];
    while (1) {
        for (int i = 0; i < current.size(); i++) {
            if (found[i] == 2) continue;
            Node& c = current[i];

            string name;
            name = instructions[sum%instructions.size()] == Dir::Left ? c.left : c.right;
            if (*name.rbegin() == 'Z')  { found[i]++; if (found[i] == 1) path_size[i].first = sum+1; else path_size[i].second = sum+1; }
            c = nodes[name];
        }
        sum ++;

        if (fold_left(found | std::views::transform([](auto v) { return v == 2;}), true, std::logical_and<bool>())) break;
    }

    uint64_t result = fold_left(path_size, 1, [](auto l, auto r) { return (uint64_t)std::lcm((uint64_t)l, (uint64_t)r.first); });
    for_each(path_size, [](auto v) { std::cout << v.first << " " << v.second << std::endl; });

    std::cout << result << std::endl;

    return 0;
}
