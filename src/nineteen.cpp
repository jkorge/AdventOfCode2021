#include "nineteen.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;


Nineteen::Nineteen(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Nineteen");
    tbl::header("Result", "Time");

    uint64_t t0 = Tempus::time();
    int x = this->part_one();
    uint64_t t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    t0 = Tempus::time();
    x = this->part_two();
    t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    tbl::sep();
    std::cout << std::endl;

    this->fin.close();
}

Nineteen::~Nineteen() = default;

void Nineteen::load(){
    std::string s;
    RefFrame r;
    Position p;
    while(!this->fin.eof()){
        std::getline(this->fin, s);
        if(!s.size()){
            this->scanners.push_back(r);
            r.clear();
            continue;
        }
        else
        if(s.substr(0,3) == "---"){ continue; }

        int x, y;
        x = s.find(',');
        y = s.find(',', x+1);
        p[0] = std::stoi(s.substr(0, x).data());
        p[1] = std::stoi(s.substr(x+1, y-x).data());
        p[2] = std::stoi(s.substr(y+1).data());
        r.insert({p[0], p[1], p[2]});

    }
    this->scanners.push_back(r);
}

Position Nineteen::get_deltas(const RefFrame& scanner, const Position& p, std::vector<Position>& deltas, int r){
    deltas.clear();
    const auto& [rx, ry, rz, sx, sy, sz] = this->rot.values[r];
    for(const auto& [x, y, z] : scanner){
        Position p2 = {x, y, z};
        deltas.push_back({
            sx * (p2[rx] - p[rx]),
            sy * (p2[ry] - p[ry]),
            sz * (p2[rz] - p[rz]),
        });
    }
    return {-sx * p[rx], -sy * p[ry], -sz * p[rz]};
}

bool Nineteen::analyze(const RefFrame& scanner){
    if(this->map.empty()){
        for(const auto& p : scanner){
            this->map.insert(p);
        }
        this->origins.push_back({0,0,0});
        return true;
    }

    std::vector<Position> deltas;
    for(const auto& [px, py, pz] : scanner){
        for(int r=0; r<this->rot.values.size(); ++r){
            Position sp = this->get_deltas(scanner, {px, py, pz}, deltas, r);

            for(const auto& [x, y, z] : this->map){
                int matches = 0;
                for(int i=0; i<deltas.size(); ++i){
                    matches += this->map.count({
                        deltas[i][0] + x,
                        deltas[i][1] + y,
                        deltas[i][2] + z
                    });
                }
                if(matches >= 12){
                    for(int i=0; i<deltas.size(); ++i){
                        this->map.insert({
                            deltas[i][0] + x,
                            deltas[i][1] + y,
                            deltas[i][2] + z
                        });
                    }
                    this->origins.push_back({sp[0] + x, sp[1] + y, sp[2] + z});
                    return true;
                }
            }
        }
    }
    return false;
}

int manhattan_distance(const Position& p, const Position& q){
    int res = 0;
    for(int i=0; i<3; ++i){
        res += std::abs(p[i] - q[i]);
    }
    return res;
}

int Nineteen::part_one(){

    std::set<int> unanalyzed;
    for(int i=0; i<this->scanners.size(); ++i){ unanalyzed.insert(i); }

    int f = 0;
    while(!unanalyzed.empty()){
        for(int i=0; i<this->scanners.size(); ++i){
            if(unanalyzed.count(i) && this->analyze(this->scanners[i])){
                unanalyzed.erase(i);
            }
        }
    }

    return this->map.size();
}

int Nineteen::part_two(){
    int res = INT_MIN;
    for(int i=0; i<this->origins.size(); ++i){
        for(int j=0; j<this->origins.size(); ++j){
            if(i == j){ continue; }
            res = std::max(res, manhattan_distance(this->origins[i], this->origins[j]));
        }
    }
    return res;
}
