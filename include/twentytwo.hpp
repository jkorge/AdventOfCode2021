#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

class TwentyTwo{
public:
    typedef std::array<int, 6> bbox;

    class Cube{
    public:
        bool state;
        bbox bounds;
        std::vector<Cube> remove;

        Cube() = default;
        Cube(bbox limits, bool s) : bounds(limits), state(s) {}

        bool overlap(const Cube&) const;
        bool contains(const Cube&) const;
        uint64_t volume() const;
        Cube intersection(const Cube&) const;
        void combine(const Cube&);
        uint64_t final_volume() const;
    };


    struct Instruction{
        bbox limits;
        bool state;
    };

private:
    std::ifstream fin;
    std::vector<Instruction> instructions;

    void load();

public:
    TwentyTwo(const std::string&);

    int part_one();
    uint64_t part_two();
};

