#include "one.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;

One::One(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day One");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
    this->fin.close();
}

void One::load(){
    int x;
    while(!this->fin.eof()){
        this->fin >> x;
        this->measurements.emplace_back(x);
    }
}

int One::part_one(){
    int res = 0;
    for(int i=1; i<this->measurements.size(); ++i){
        if(this->measurements[i] > this->measurements[i-1]){ ++res; }
    }
    return res;
}

int One::part_two(){
    std::array<int, 3> window;
    int sum, prev = INT_MAX, res = 0;
    for(int i=0; i<this->measurements.size(); ++i){
        window[i%3] = this->measurements[i];
        if(i >= 2){
            sum = window[0] + window[1] + window[2];
            if(sum > prev){ ++res; }
            prev = sum;
        }
    }
    return res;
}