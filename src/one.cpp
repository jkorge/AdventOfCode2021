#include "one.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

One::One(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day One");
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

void One::load(){
    int x;
    while(!this->fin.eof()){
        this->fin >> x;
        this->measurements.emplace_back(x);
    }
}

int One::part_one(){
    int res = 0, N = this->measurements.size();
    for(int i=1; i<N; ++i){
        if(this->measurements[i] > this->measurements[i-1]){ ++res; }
    }
    return res;
}

int One::part_two(){
    int sum,
        prev = INT_MAX,
        res = 0,
        N = this->measurements.size();

    std::array<int, 3> window;
    for(int i=0; i<N; ++i){
        window[i%3] = this->measurements[i];
        if(i >= 2){
            sum = window[0] + window[1] + window[2];
            if(sum > prev){ ++res; }
            prev = sum;
        }
    }
    return res;
}