#include "seventeen.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

Seventeen::Seventeen(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Seventeen");
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

void Seventeen::load(){
    std::string x, y;
    std::getline(this->fin, x);

    int a = x.find('x'),
        b = x.find(','),
        c = x.find('y');

    y = x.substr(c);
    x = x.substr(a, b-a);

    a = x.find('.');
    int x0 = std::stoi(x.substr(2, a-2).data()),
        x1 = std::stoi(x.substr(a+2).data());
    this->target.x0 = std::min(x0, x1);
    this->target.x1 = std::max(x0, x1);
    
    a = y.find('.');
    int y0 = std::stoi(y.substr(2, a-2).data()),
        y1 = std::stoi(y.substr(a+2).data());
    this->target.y0 = std::min(y0, y1);
    this->target.y1 = std::max(y0, y1);

}

inline bool Seventeen::within(const int& x, const int& y) const{ return (x >= this->target.x0 && x <= this->target.x1) && (y >= this->target.y0 && y <= this->target.y1); }

inline bool Seventeen::beyond(const int& x, const int& y) const{ return x > this->target.x1 || y < this->target.y0; }

bool Seventeen::valid(int vx, int vy){

    int x = 0, y = 0;
    while(!this->within(x, y) && !this->beyond(x, y)){
        x += vx;
        y += vy;
        if(vx){ --vx; }
        --vy;
    }
    return within(x, y);
}

int Seventeen::part_one(){
    /*
        @ t = 0
            y = 0
            vy = vy_initial > 0
        @ t = 1
            y = vy_initial
            vy = vy_initial - 1
        ...
        @ t = T
            y = vy_initial
            vy = -vy_initial
        @ t = T+1
            y = 0
            vy = -(vy_initial + 1)
        @ t = T+2
            y = -(vy_initial + 1)

        if vy_initial >= abs(target.y0)
        then y @ T+2 < target.y0 (ie. past the lower bound of target region)
        => vy_initial_max = abs(target.y0) - 1

        height achieved for any positive vy_initial is the sum from 0 to vy_initial
    */

    int vy = std::abs(this->target.y0)-1;
    return vy * (vy+1) / 2;
}

int Seventeen::part_two(){
    /*
        Distance traveled in x is the sum from 0 to vx_initial
        Seek:
            vxi * (vxi + 1) / 2 >= target.x0
        Rearrange terms, and using quadradtic formula to obtain vx_initial_min

        vx_initial_max is target.x1 (just kick it straight to the outer edge of target region)

        vy_initial_max is described in part_one

        vy_initial_min is target.y0 (just kick it straight to the outer edge of target region)
    */

    int vx_min = std::ceil((-1. + std::sqrt(1. + 8.*float(this->target.x0))) / 2.), // for any vy
        vx_max = this->target.x1,
        vy_min = this->target.y0,
        vy_max = std::abs(this->target.y0)-1;

    int res = 0;
    for(int vx=vx_min; vx<=vx_max; ++vx){
        for(int vy=vy_min; vy<=vy_max; ++vy){
            if(this->valid(vx, vy)){ ++res; }
        }
    }
    return res;
}
