#include "fourteen.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

Fourteen::Fourteen(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Fourteen");
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

void Fourteen::load(){
    std::string x;
    std::getline(this->fin, this->templ);
    std::getline(this->fin, x); // discard empty line
    while(!this->fin.eof()){
        std::getline(this->fin, x);
        this->insertion_rules[x.substr(0,2)] = x.back();
    }
}

template<typename T>
T Fourteen::min_max_diff(const std::unordered_map<char, T>& counts){
    T max = 0, min = UINT64_MAX;
    for(typename std::unordered_map<char, T>::const_iterator it=counts.cbegin(); it!=counts.cend(); ++it){
        max = std::max(it->second, max);
        min = std::min(it->second, min);
    }
    return max - min;
}

std::unordered_map<std::string, uint64_t> Fourteen::pair_counter(int N){
    std::unordered_map<std::string, uint64_t> pair_counts;
    for(int i=0; i<this->templ.size()-1; ++i){ ++pair_counts[this->templ.substr(i, 2)]; }
    for(int n=0; n<N; ++n){
        std::unordered_map<std::string ,uint64_t> new_pair_counts;
        std::string new_pair;
        for(std::unordered_map<std::string, uint64_t>::iterator it=pair_counts.begin(); it!=pair_counts.end(); ++it){
            new_pair_counts[it->first.substr(0, 1) + this->insertion_rules[it->first]] += it->second;
            new_pair_counts[this->insertion_rules[it->first] + it->first.substr(1, 1)] += it->second;
        }
        pair_counts = std::move(new_pair_counts);
    }
    return pair_counts;
}

std::unordered_map<char, uint64_t> Fourteen::char_counter(const std::unordered_map<std::string, uint64_t>& pair_counts){
    std::unordered_map<char, uint64_t> counts;
    for(std::unordered_map<std::string, uint64_t>::const_iterator it=pair_counts.cbegin(); it!=pair_counts.cend(); ++it){ counts[it->first[0]] += it->second; }
    ++counts[this->templ.back()];
    return counts;
}

int Fourteen::part_one(){
    std::unordered_map<std::string, uint64_t> pair_counts = this->pair_counter(10);
    std::unordered_map<char, uint64_t> counts = this->char_counter(pair_counts);
    return this->min_max_diff(counts);
}

uint64_t Fourteen::part_two(){
    std::unordered_map<std::string, uint64_t> pair_counts = this->pair_counter(40);
    std::unordered_map<char, uint64_t> counts = this->char_counter(pair_counts);
    return this->min_max_diff(counts);
}
