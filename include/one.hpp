#include <array>
#include <climits>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>


class One{

    std::ifstream fin;
    std::vector<int> measurements;

    void load();

public:
    One(const std::string&);

    int part_one();
    int part_two();
};