#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef enum direction{ forward=0, down, up} direction;


class Two{

    std::ifstream fin;
    std::array<int, 3> position;
    std::vector<std::pair<direction, int> > data;

    void load();

public:
    Two(const std::string&);

    int part_one();
    int part_two();
};