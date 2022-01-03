#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <vector>

typedef enum sc{ empty=0, south=-1, east=1 } sc;

class TwentyFive{

    typedef std::vector<std::vector<sc> > scmap;

    std::ifstream fin;
    scmap map;

    void load();

    template<sc S>
    bool check_(const scmap&, int, int) const;
    bool check(const scmap&, int, int) const;

    template<sc S>
    void move_(scmap&, int, int) const;
    void move(scmap&, int, int) const;

    template<sc S>
    scmap step_(const scmap&) const;
    scmap step(const scmap&) const;

public:
    TwentyFive(const std::string&);

    int part_one();
    int part_two();
};

