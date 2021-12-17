#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Five{

    struct coord{
        int x, y;
        coord() : x(-1), y(-1) {}
        coord(int cx, int cy) : x(cx), y(cy) {}
    };

    std::ifstream fin;
    std::vector<std::pair<coord, coord> > endpoints;
    int xlim, ylim;

    void load();

public:
    Five(const std::string&);

    int part_one();
    int part_two();
};