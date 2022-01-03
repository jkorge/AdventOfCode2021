#include "twentyone.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

TwentyOne::TwentyOne(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Twenty-One");
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

void TwentyOne::load(){
    std::string x;
    std::getline(this->fin, x);
    this->p1start = x.back() - '0';
    std::getline(this->fin, x);
    this->p2start = x.back() - '0';
}

constexpr std::array<uint8_t, 27> _dirac_die(){
    std::array<uint8_t, 27> res = {0};
    for(int i=0; i<3; ++i){
        for(int j=0; j<3; ++j){
            for(int k=0; k<3; ++k){
                res[9*i + 3*j + k] += i + j + k + 3;
            }
        }
    }
    return res;
}

constexpr std::array<uint8_t, 27> dirac_die = _dirac_die();

std::array<int, 3> TwentyOne::deterministic(std::array<int, 2> positions){
    int n, t;
    std::array<int, 2> scores = {0};
    for(n=0, t=0; scores[0] < 1000 && scores[1] < 1000; t=(t+1)%2){
        for(int i=0; i<3; ++i){ positions[t] += (n++ % 100) + 1; }
        positions[t] = (positions[t] - 1) % 10 + 1;
        scores[t] += positions[t];
    }
    return {scores[0], scores[1], n};
}

std::array<uint64_t, 2> TwentyOne::quantum(TwentyOne::state S){
    static std::map<state, std::array<uint64_t, 2> > memo;

    if(memo.find(S) != memo.end()){ return memo[S]; }

    auto [_p1, _p2, _s1, _s2] = S;
    if(_s1 >= 21){
        memo[S] = {1, 0};
        return memo[S];
    }
    else
    if(_s2 >= 21){
        memo[S] = {0, 1};
        return memo[S];
    }

    uint64_t w1 = 0, w2 = 0;
    for(int i=0; i<27; ++i){
        auto [p1, p2, s1, s2] = S;
        p1 = (p1 + dirac_die.at(i) - 1) % 10 + 1;
        s1 += p1;
        std::array<uint64_t, 2> outcome = this->quantum({p2, p1, s2, s1});
        w1 += outcome[1];
        w2 += outcome[0];
    }

    memo[S] = {w1, w2};
    return memo[S];
}

int TwentyOne::part_one(){
    std::array<int, 3> outcome = this->deterministic({this->p1start, this->p2start});
    return std::min(outcome[0], outcome[1]) * outcome[2];
}

uint64_t TwentyOne::part_two(){
    std::array<uint64_t, 2> outcomes = this->quantum({this->p1start, this->p2start, 0, 0});
    return std::max(outcomes[0], outcomes[1]);
}
