#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <tuple>

typedef std::array<int, 3> Position;
typedef std::tuple<int, int, int> Measurement;
typedef std::set<Measurement> RefFrame;
typedef std::array<std::array<int, 3>, 3> Matrix;

struct Rotations{
    std::vector<std::tuple<int, int, int, int, int, int> > values;

    Rotations() {

        std::vector<std::tuple<int, int, int, int, int, int> > ref = {
            {0, 1, 2,  1,  1,  1},
            {0, 1, 2,  1, -1, -1},
            {0, 2, 1,  1,  1, -1},
            {0, 2, 1,  1, -1,  1},
            {0, 1, 2, -1,  1, -1},
            {0, 1, 2, -1, -1,  1},
            {0, 2, 1, -1,  1,  1},
            {0, 2, 1, -1, -1, -1}
        };

        for(int axis=0; axis<3; ++axis){
            for(const auto& [dx, dy, dz, sx, sy, sz] : ref){
                this->values.push_back({
                    (axis + dx) % 3,
                    (axis + dy) % 3,
                    (axis + dz) % 3,
                    sx, sy, sz
                });
            }
        }
    }

};

class Nineteen{

    std::ifstream fin;
    std::vector<RefFrame> scanners;
    std::vector<Position> origins;
    RefFrame map;
    const Rotations rot;

    void load();
    Position get_deltas(const RefFrame&, const Position&, std::vector<Position>&, int);
    bool analyze(const RefFrame&);

public:
    Nineteen(const std::string&);
    ~Nineteen();

    int part_one();
    int part_two();
};

