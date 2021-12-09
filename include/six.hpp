#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Six{

    std::ifstream fin;
    std::vector<int> initial_values;

    void load();
    long long predict(int);

public:
    Six(const std::string&);

    long long part_one();
    long long part_two();
};