#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <vector>


struct square{
    int value;
    bool marked;
};

typedef std::array<std::array<square, 5>, 5> Board;

class Four{

    std::ifstream fin;
    std::vector<int> moves;
    std::vector<Board> boards;

    void load();

public:
    Four(const std::string&);

    int part_one();
    int part_two();
};