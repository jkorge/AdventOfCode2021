#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Seventeen{

    struct box{
        int x0, x1,
            y0, y1;
    };

    std::ifstream fin;
    box target;

    void load();
    bool within(const int&, const int&) const;
    bool beyond(const int&, const int&) const;
    bool valid(int, int);

public:
    Seventeen(const std::string&);

    int part_one();
    int part_two();
};

