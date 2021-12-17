#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

class Ten{

    std::ifstream fin;
    std::vector<std::string> data;

    void load();

public:
    Ten(const std::string&);

    int part_one();
    long long part_two();
};