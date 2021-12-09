#include <array>
#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

constexpr int bitwidth = 12;

class Three{

    std::ifstream fin;
    std::bitset<bitwidth> most_common, least_common;
    std::vector<std::bitset<bitwidth> > vals;

    void count();
    void load();

public:
    Three(const std::string&);

    int part_one();
    int part_two();
};