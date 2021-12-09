#include "three.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;

Three::Three(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day Three");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
    this->fin.close();
}

void Three::load(){
    this->vals.clear();
    this->fin.clear();
    this->fin.seekg(0);
    std::bitset<bitwidth> x;
    this->fin >> x;
    while(!this->fin.eof()){
        this->vals.emplace_back(x);
        this->fin >> x;
    }
}

void Three::count(){
    std::array<int, bitwidth> zero_counts = {0}, one_counts = {0};

    for(std::vector<std::bitset<bitwidth> >::iterator it=this->vals.begin(); it!=this->vals.end(); ++it){
        for(int i=0; i<bitwidth; ++i){
            if(it->test(i)){ ++one_counts[i]; }
            else{ ++zero_counts[i]; }
        }
    }

    for(int i=0; i<bitwidth; ++i){
        this->most_common[i] = one_counts[i] >= zero_counts[i];
        this->least_common[i] = zero_counts[i] > one_counts[i];
    }
}

int Three::part_one(){
    this->load();
    this->count();
    return this->most_common.to_ulong() * (~this->most_common.to_ulong() & 0b0000111111111111);
}

int Three::part_two(){
    this->load();
    for(int i=bitwidth-1; i>=0 && this->vals.size()!=1; --i){
        this->count();
        for(std::vector<std::bitset<bitwidth> >::iterator it=this->vals.begin(); it!=this->vals.end() && this->vals.size()!=1; ){
            if(it->test(i) != this->most_common.test(i)){ it = this->vals.erase(it); }
            else{ ++it; }
        }
    }
    int o2 = this->vals[0].to_ulong();

    this->load();
    for(int i=bitwidth-1; i>=0 && this->vals.size()!=1; --i){
        this->count();
        for(std::vector<std::bitset<bitwidth> >::iterator it=this->vals.begin(); it!=this->vals.end() && this->vals.size()!=1; ){
            if(it->test(i) != this->least_common.test(i)){ it = this->vals.erase(it); }
            else{ ++it; }
        }
    }
    int co2 = this->vals[0].to_ulong();

    return o2 * co2;
}