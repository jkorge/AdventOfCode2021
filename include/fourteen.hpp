#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Fourteen{

    std::ifstream fin;
    std::string templ;
    std::unordered_map<std::string, char> insertion_rules;

    void load();
    template<typename T>
    T min_max_diff(const std::unordered_map<char, T>&);
    std::unordered_map<std::string, uint64_t> pair_counter(int);
    std::unordered_map<char, uint64_t> char_counter(const std::unordered_map<std::string, uint64_t>&);

public:
    Fourteen(const std::string&);
    ~Fourteen();

    int part_one();
    uint64_t part_two();
};

