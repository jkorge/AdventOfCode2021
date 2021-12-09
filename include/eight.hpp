#include <array>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Record{
    std::array<std::string, 10> signals;
    std::array<std::string, 4> output;

    friend std::basic_istream<char>& operator>>(std::basic_istream<char>& is, Record&);
    std::unordered_map<char, std::string> decode() const;

};

class Eight{

    std::ifstream fin;
    std::vector<Record> records;

    void load();

public:
    Eight(const std::string&);

    int part_one();
    int part_two();
};