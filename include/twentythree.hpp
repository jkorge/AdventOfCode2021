#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <vector>

class TwentyThree{


    std::ifstream fin;
    std::string state;

    void load();


public:
    TwentyThree(const std::string&);

    int part_one();
    int part_two();
};

