#include "seven.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;

Seven::Seven(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day Seven");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
    this->fin.close();
}

void Seven::load(){
    int x;
    while(!this->fin.eof()){
        this->fin >> x;
        this->initial_values.emplace_back(x);
        this->xlim = std::max(this->xlim, x);
        this->fin.get();
    }
}

int Seven::fuel_used(const int dst, const bool decay) const{
    int res = 0, distance;
    for(std::vector<int>::const_iterator it=this->initial_values.cbegin(); it!=this->initial_values.cend(); ++it){
        distance = std::abs(*it - dst);
        res += decay ? (distance * (distance + 1) / 2) : distance;
    }
    return res;
}

int Seven::optimize(const bool decay){
    int fuel_sp = this->fuel_used(0, decay),
        fuel_ep = this->fuel_used(this->xlim, decay),
        sp = 0, ep = this->xlim,
        fuel_mp, mp, res = -1;

    while(true){
        mp = (sp + ep) / 2;
        fuel_mp = this->fuel_used(mp, decay);
        if(fuel_sp - fuel_mp < fuel_ep - fuel_mp){
            ep = mp;
            fuel_ep = fuel_mp;
        }
        else{
            sp = mp;
            fuel_sp = fuel_mp;
        }

        if(ep - sp <= 1){ res = std::min(fuel_sp, fuel_ep); break; }
    }

    return res;
}

int Seven::part_one(){ return this->optimize(false); }

int Seven::part_two(){ return this->optimize(true); }