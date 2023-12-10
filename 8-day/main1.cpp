#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <stdint.h>

typedef std::string string;

struct Node {
    string left;
    string right;
};

enum Dir { Left, Right };

int main() {

    std::ifstream input("../input.txt");

    string instructions_str; input >> instructions_str;

    std::vector<Dir> instructions;
    for (char c : instructions_str) { instructions.push_back(c == 'L' ? Dir::Left : Dir::Right); }

    string node_name;
    std::unordered_map<string, Node> nodes;
    while (input >> node_name) {
        string eqs; string left; string right;
        input >> eqs;
        input >> left;
        input >> right;
        std::cout << node_name << " " << left << " " << right << std::endl;
        nodes[node_name] = { left.substr(1, left.size()-2), right.substr(0, right.size()-1) };
    }

    //for (auto n : nodes) {
    //    std::cout << n.first << " " << n.second.left << " " << n.second.right << std::endl;
    //}

    uint32_t sum = 0;
    Node current = nodes["AAA"];

    while (1) {
        string name;
        name = instructions[sum++%instructions.size()] == Dir::Left ? current.left : current.right;
        sum ++;
        if (name == "ZZZ") break;
        current = nodes[name];
    }

    std::cout << sum << std::endl;

    return 0;
}
