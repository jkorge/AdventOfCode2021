#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Thirteen{

public:
    struct coord{
        int x;
        int y;
        coord() {}
        coord(int cx, int cy) : x(cx), y(cy) {}
    };

    typedef std::pair<bool, int> fold;

    struct Instructions{
        std::vector<coord> coords;
        std::vector<fold> folds;
    };

    typedef std::vector<std::vector<bool> > grid;

private:
    std::ifstream fin;
    Instructions ins;

    void load();

public:
    Thirteen(const std::string&);
    ~Thirteen();

    int part_one();
    std::string part_two();
};

