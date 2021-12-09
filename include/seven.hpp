#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Seven{

    std::ifstream fin;
    std::vector<int> initial_values;
    int xlim = -1;

    void load();
    int fuel_used(const int, const bool=false) const;
    int optimize(const bool);

public:
    Seven(const std::string&);

    int part_one();
    int part_two();
};