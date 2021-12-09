#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Heightmap : std::vector<std::vector<int> >{
    std::vector<std::pair<int, int> > lowpoints;
};

class Nine{

    std::ifstream fin;
    Heightmap heightmap;

    void load();
    int basin_size(int, int, std::vector<std::vector<bool> >&);

public:
    Nine(const std::string&);

    int part_one();
    int part_two();
};