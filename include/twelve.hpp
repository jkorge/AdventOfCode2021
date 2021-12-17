#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

struct Cave{
    bool small;
    std::string name;
    std::vector<Cave*> tunnels;
    Cave() : small(false) {}
    Cave(std::string n) : name(n), small(false) {}
    Cave(std::string n, bool s) : name(n), small(s) {}
    ~Cave() {}
};

typedef std::vector<std::string> path;
typedef std::unordered_map<std::string, Cave*> cavelist;

class Twelve{

    std::ifstream fin;
    Cave start{"start"}, end{"end"};
    cavelist caves{
        {start.name, &start},
        {end.name, &end}
    };

    void load();

public:
    Twelve(const std::string&);
    ~Twelve();

    int part_one();
    int part_two();
};

