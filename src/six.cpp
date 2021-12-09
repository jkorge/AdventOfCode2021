#include "six.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;

Six::Six(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day Six");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
    this->fin.close();
}

void Six::load(){
    int x;
    while(!this->fin.eof()){
        this->fin >> x;
        this->initial_values.emplace_back(x);
        this->fin.get();
    }
}

long long Six::predict(const int ndays){

    std::unordered_map<int, long long> day_counts;
    for(std::vector<int>::const_iterator it=this->initial_values.cbegin(); it!=this->initial_values.cend(); ++it){
        ++day_counts[*it];
    }

    for(int i=0; i<ndays; ++i){
        long long bak = day_counts[0];
        for(int j=0; j<8; ++j){
            day_counts[j] = day_counts[j+1];
        }
        day_counts[8] = bak;
        day_counts[6] += bak;
    }

    long long res = 0;
    for(int j=0; j<9; ++j){ res += day_counts[j]; }
    return res;

}

long long Six::part_one(){ return predict(80); }

long long Six::part_two(){ return predict(256); }