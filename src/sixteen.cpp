#include "sixteen.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

const std::unordered_map<char, std::array<bool, 4> > hexmap{
    {'0', {false, false, false, false}},
    {'1', {false, false, false, true }},
    {'2', {false, false, true , false}},
    {'3', {false, false, true , true }},
    {'4', {false, true , false, false}},
    {'5', {false, true , false, true }},
    {'6', {false, true , true , false}},
    {'7', {false, true , true , true }},
    {'8', {true , false, false, false}},
    {'9', {true , false, false, true }},
    {'A', {true , false, true , false}},
    {'B', {true , false, true , true }},
    {'C', {true , true , false, false}},
    {'D', {true , true , false, true }},
    {'E', {true , true , true , false}},
    {'F', {true , true , true , true }},
};

inline uint64_t pack2int(const packet& p){
    uint64_t res = 0;
    int N = p.size();
    for(int j=1; j<=N; ++j){ res |= p.at(N-j) ? (1ULL << j-1) : 0ULL; }
    return res;
}

inline uint64_t pack2int(typename packet::const_iterator& it, int N){
    uint64_t res = 0;
    for(int j=1; j<=N; ++j){ res |= (*it++) ? (1ULL << N-j) : 0ULL; }
    return res;
}

int version(typename packet::const_iterator& it){
    int res = 0;
    for(int i=0; i<3; ++i){ res |= (*it++) ? (1 << 2-i) : 0; }
    return res;
}

int type(typename packet::const_iterator& it){
    int res = 0;
    for(int i=0; i<3; ++i){ res |= (*it++) ? (1 << 2-i) : 0; }
    return res;
}

uint64_t literal(typename packet::const_iterator& it){
    int i = 0;
    bool done = false;
    packet value;
    while(!done){
        done = !(*it++);
        for(int j=0; j<4; ++j){ value.push_back(*it++); }
    }

    uint64_t res = pack2int(value);
    return res;
}

inline uint64_t sum(const std::vector<uint64_t>& values){
    uint64_t res = 0;
    for(std::vector<uint64_t>::const_iterator it=values.cbegin(); it!=values.cend(); ++it){ res += *it; }
    return res;
}

inline uint64_t prod(const std::vector<uint64_t>& values){
    uint64_t res = 1;
    for(std::vector<uint64_t>::const_iterator it=values.cbegin(); it!=values.cend(); ++it){ res *= *it; }
    return res;
}

inline uint64_t min(const std::vector<uint64_t>& values){
    uint64_t res = UINT64_MAX;
    for(std::vector<uint64_t>::const_iterator it=values.cbegin(); it!=values.cend(); ++it){ res = std::min(res, (uint64_t)*it); }
    return res;
}

inline uint64_t max(const std::vector<uint64_t>& values){
    uint64_t res = 0;
    for(std::vector<uint64_t>::const_iterator it=values.cbegin(); it!=values.cend(); ++it){ res = std::max(res, (uint64_t)*it); }
    return res;
}

inline int gt(const std::vector<uint64_t>& values){ return (values.at(0) > values.at(1)) ? 1 : 0; }

inline int lt(const std::vector<uint64_t>& values){ return (values.at(0) < values.at(1)) ? 1 : 0; }

inline int eq(const std::vector<uint64_t>& values){ return (values.at(0) == values.at(1)) ? 1 : 0; }

uint64_t parse(typename packet::const_iterator&);

uint64_t op(typename packet::const_iterator& it, int type){

    // Get size of subpackets
    int itype = (*it++) ? 11 : 15;
    uint64_t length = pack2int(it, itype);

    // Collect values of subpackets
    std::vector<uint64_t> values;
    if(itype == 15){
        typename packet::const_iterator it_stop = it + length;
        while(it <= it_stop - 11){ values.push_back(parse(it)); }
        it = it_stop;
    }
    else{
        for(uint64_t i=0; i<length; ++i){ values.push_back(parse(it)); }
    }

    // Apply op over values
    uint64_t res;
    switch(type){
        case 0:  res = sum(values);  break;
        case 1:  res = prod(values); break;
        case 2:  res = min(values);  break;
        case 3:  res = max(values);  break;
        case 5:  res = gt(values);   break;
        case 6:  res = lt(values);   break;
        case 7:
        default: res = eq(values);
    }

    return res;
}

uint64_t parse(typename packet::const_iterator& it){
    int v = version(it),
        t = type(it);
    return t == 4 ? literal(it) : op(it, t);
}

int parse_version(typename packet::const_iterator& it){
    int v = version(it),
        t = type(it);

    if(t == 4){ literal(it); }
    else{

        int itype = (*it++) ? 11 : 15;
        uint64_t length = pack2int(it, itype);

        std::vector<uint64_t> values;
        if(itype == 15){
            typename packet::const_iterator it_stop = it + length;
            while(it <= it_stop - 11){ values.push_back(parse_version(it)); }
            it = it_stop;
        }
        else{
            for(uint64_t i=0; i<length; ++i){ values.push_back(parse_version(it)); }
        }

        for(int i=0; i<values.size(); ++i){ v += values[i]; }
    }
    return v;
}

Sixteen::Sixteen(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Sixteen");
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

void Sixteen::load(){
    std::string x;
    std::getline(this->fin, x);
    for(int i=0; i<x.size(); ++i){
        std::array<bool, 4> _bits = hexmap.at(x[i]);
        for(int j=0; j<4; ++j){ this->bits.emplace_back(_bits[j]); }
    }
}

int Sixteen::part_one(){
    typename packet::const_iterator it = this->bits.cbegin();
    return parse_version(it);
}

uint64_t Sixteen::part_two(){
    typename packet::const_iterator it = this->bits.cbegin();
    return parse(it);
}
