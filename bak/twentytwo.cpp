#include "twentytwo.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

TwentyTwo::TwentyTwo(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day TwentyTwo");
    tbl::header("Result", "Time");

    uint64_t t0 = Tempus::time();
    int x = this->part_one();
    uint64_t t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    t0 = Tempus::time();
    uint64_t y = this->part_two();
    t1 = Tempus::time();
    tbl::row(y, Tempus::strtime(t1-t0));

    tbl::sep();
    std::cout << std::endl;

    this->fin.close();
}

void TwentyTwo::load(){
    std::string x;
    bool s;
    bbox limits;
    const char chars[3] = {'x', 'y', 'z'};
    while(!this->fin.eof()){
        std::getline(this->fin, x);
        s = x.substr(0, 2) == "on";
        
        int a, b, c;
        for(int i=0; i<3; ++i){
            a = x.find(chars[i]);
            b = x.find('.', a);
            c = x.find(',', a);

            limits[2*i    ] = std::stoi(x.substr(a+2, b-a-2).data());
            limits[2*i + 1] = std::stoi(x.substr(b+2, c-b-2).data());
        }

        this->instructions.push_back({{limits[0], limits[1], limits[2], limits[3], limits[4], limits[5]}, s});
    }
}

inline bool overlap1d(const std::array<int, 2> a, const std::array<int, 2> b){ return std::max(a.at(0), b.at(0)) <= std::min(a.at(1), b.at(1)); }

inline std::array<int, 2> intersection1d(const std::array<int, 2> a, const std::array<int, 2> b){ return {std::max(a.at(0), b.at(0)), std::min(a.at(1), b.at(1))}; }

bool TwentyTwo::Cube::overlap(const TwentyTwo::Cube& other) const{
    bool res = true;
    for(int i=0; i<6; i+=2){ res &= overlap1d({this->bounds.at(i), this->bounds.at(i+1)}, {other.bounds.at(i), other.bounds.at(i+1)}); }
    return res;
}

uint64_t TwentyTwo::Cube::volume() const{
    uint64_t res = 1;
    for(int i=0; i<6; i+=2){ res *= this->bounds.at(i+1) - this->bounds.at(i) + 1; }
    return res;
}

TwentyTwo::Cube TwentyTwo::Cube::intersection(const TwentyTwo::Cube& other) const{
    bbox res;
    std::array<int, 2> res1d;
    for(int i=0; i<6; i+=2){
        res1d = intersection1d({this->bounds.at(i), this->bounds.at(i+1)}, {other.bounds.at(i), other.bounds.at(i+1)});
        res[i] = res1d[0];
        res[i+1] = res1d[1];
    }
    return Cube(res, false);
}

void TwentyTwo::Cube::combine(const TwentyTwo::Cube& other){
    // Add intersection with other to list of cubes with volume to be removed from this one in `final_volume`
    if(this->overlap(other)){
        Cube intersect = this->intersection(other);
        for(int i=0; i<this->remove.size(); ++i){
            // Recurse for cubes already flagged for removal from this
            // ie. if this new intersection also intersects with other intersections, avoid double counting the removal
            this->remove[i].combine(intersect);
        }
        this->remove.push_back(intersect);
    }
}

uint64_t TwentyTwo::Cube::final_volume() const{
    uint64_t res = this->volume(),
             rem = 0;
    for(int i=0; i<this->remove.size(); ++i){
        rem += this->remove.at(i).final_volume();
    }
    return res - rem;
}

int TwentyTwo::part_one(){

    std::vector<Cube> cubes;
    Cube roi({-50, 50, -50, 50, -50, 50}, false),
         c;
    for(std::vector<Instruction>::const_iterator it=this->instructions.cbegin(); it!=this->instructions.cend(); ++it){

        // Keep if within ROI
        c = Cube(it->limits, it->state);
        if(!c.overlap(roi)){ continue; }
        c = c.intersection(roi);

        // Combine with other saved regions (if necessary)
        for(int j=0; j<cubes.size(); ++j){ cubes[j].combine(c); }

        // Only save 'on' regions
        if(it->state){ cubes.push_back(c); }
    }

    int res = 0;
    for(std::vector<Cube>::iterator it=cubes.begin(); it!=cubes.end(); ++it){ res += it->final_volume(); }
    return res;
}

uint64_t TwentyTwo::part_two(){

    std::vector<Cube> cubes;
    Cube c;
    for(std::vector<Instruction>::const_iterator it=this->instructions.cbegin(); it!=this->instructions.cend(); ++it){

        c = Cube(it->limits, it->state);

        // Combine with other saved regions (if necessary)
        for(int j=0; j<cubes.size(); ++j){ cubes[j].combine(c); }

        // Only save 'on' regions
        if(it->state){ cubes.push_back(c); }
    }

    uint64_t res = 0;
    for(std::vector<Cube>::iterator it=cubes.begin(); it!=cubes.end(); ++it){ res += it->final_volume(); }
    return res;
}
