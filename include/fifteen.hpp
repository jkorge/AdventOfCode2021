#include <array>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <queue>

class Fifteen{

    struct Step{
        int x, y, cost;
    };

    std::ifstream fin;
    std::vector<std::vector<uint8_t> > grid;

    void load();

public:
    Fifteen(const std::string&);

    int part_one();
    int part_two();
};

