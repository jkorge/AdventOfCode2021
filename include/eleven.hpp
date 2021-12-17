#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

typedef std::array<std::array<uint8_t, 10>, 10> energymap;
typedef std::array<std::array<bool, 10>, 10> flashmap;

class Eleven{

    std::ifstream fin;
    energymap energy;

    void load();
    void one_for_all();
    void reset();
    bool all_marked(const flashmap&);
    void xflash(int, int, flashmap&);

public:
    Eleven(const std::string&);

    int part_one();
    int part_two();
};

