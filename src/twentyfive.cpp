#include "twentyfive.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

TwentyFive::TwentyFive(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day TwentyFive");
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

void TwentyFive::load(){
    std::string x;
    while(!this->fin.eof()){
        std::getline(this->fin, x);
        std::vector<sc> row;
        for(int i=0; i<x.size(); ++i){
            switch(x[i]){
                case 'v': row.push_back(south); break;
                case '>': row.push_back(east); break;
                default:  row.push_back(empty);
            }
        }
        this->map.push_back(row);
    }
}

template<>
bool TwentyFive::check_<south>(const TwentyFive::scmap& m, int i, int j) const{ return m.at((i+1) % m.size()).at(j) == empty; }

template<>
bool TwentyFive::check_<east>(const TwentyFive::scmap& m, int i, int j) const{ return m.at(i).at((j+1) % m[0].size()) == empty; }

template<>
bool TwentyFive::check_<empty>(const TwentyFive::scmap& m, int i, int j) const{ return false; }

inline bool TwentyFive::check(const TwentyFive::scmap& m, int i, int j) const{
    switch(m.at(i).at(j)){
        case south: return this->check_<south>(m, i, j);
        case east:  return this->check_<east >(m, i, j);
        default:    return this->check_<empty>(m, i, j);
    }
}

template<>
void TwentyFive::move_<south>(TwentyFive::scmap& m, int i, int j) const{
    m[i][j] = empty;
    m[(i+1) % m.size()][j] = south;
}

template<>
void TwentyFive::move_<east>(TwentyFive::scmap& m, int i, int j) const{
    m[i][j] = empty;
    m[i][(j+1) % m[0].size()] = east;
}

template<>
void TwentyFive::move_<empty>(TwentyFive::scmap& m, int i, int j) const{ m[i][j] = empty; }

inline void TwentyFive::move(TwentyFive::scmap& m, int i, int j) const{
    switch(m.at(i).at(j)){
        case south: this->move_<south>(m, i, j); break;
        case east:  this->move_<east >(m, i, j); break;
        default:    this->move_<empty>(m, i, j); break;
    }
}

template<sc S>
TwentyFive::scmap TwentyFive::step_(const TwentyFive::scmap& m) const{
    const int X = m.size(), Y = m[0].size();
    scmap res = m;
    for(int i=0; i<X; ++i){
        for(int j=0; j<Y; ++j){
            if(m.at(i).at(j) == S && this->check(m, i, j)){ this->move(res, i, j); }
        }
    }
    return res;
}

TwentyFive::scmap TwentyFive::step(const TwentyFive::scmap& m) const{
    scmap res = this->step_<east>(m);
    res = this->step_<south>(res);
    return res;
}

int TwentyFive::part_one(){
    int res = 0;
    scmap m = this->map, prev;
    for(; prev != m; ++res){
        prev = m;
        m = this->step(m);
    }

    return res;
}

int TwentyFive::part_two(){
    return -1;
}
