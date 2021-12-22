#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include <vector>

class TwentyOne{
public:
    typedef std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> state;

private:
    std::ifstream fin;
    int p1start, p2start;

    void load();
    std::array<int, 3> deterministic(std::array<int, 2>);
    std::array<uint64_t, 2> quantum(state);

public:
    TwentyOne(const std::string&);

    int part_one();
    uint64_t part_two();
};

