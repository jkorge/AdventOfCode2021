#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef std::vector<bool> packet;

class Sixteen{

    std::ifstream fin;
    packet bits;

    void load();

public:
    Sixteen(const std::string&);

    int part_one();
    uint64_t part_two();
};

