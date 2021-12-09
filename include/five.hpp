#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

typedef struct coord{
    int x, y;
    coord() : x(-1), y(-1) {}
    coord(int _x, int _y) : x(_x), y(_y) {}
    bool operator==(const coord& other) const{ return this->x == other.x && this->y == other.y; }
    bool operator<(const coord& other) const{ return this->x < other.x && this->y < other.y; }
} coord;


struct CoordHash{
    std::size_t operator()(const coord& c) const{
        return std::hash<int>()(c.x) ^ (std::hash<int>()(c.y) << 1);
    }
};

class Five{

    std::ifstream fin;
    std::vector<std::pair<coord, coord> > endpoints;

    void load();

public:
    Five(const std::string&);

    int part_one();
    int part_two();
};