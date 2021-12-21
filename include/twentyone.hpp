#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include <vector>

class TwentyOne{

    std::ifstream fin;
    int p1start, p2start;

    void load();
    int deterministic();
    std::array<uint64_t, 2> quantum(uint8_t, uint8_t, uint8_t, uint8_t);

public:
    TwentyOne(const std::string&);

    int part_one();
    uint64_t part_two();
};

