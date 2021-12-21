#include "twentyone.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

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

inline void move_deterministic(int& loc, int& n, int& score){
    for(int i=0; i<3; ++i){ loc += (n++ % 100) + 1; }
    loc = loc % 10;
    if(loc == 0){ loc = 10; }
    score += loc;
}

constexpr std::array<uint8_t, 27> _mq(){
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

constexpr std::array<uint8_t, 27> mq = _mq();

int TwentyOne::deterministic(){
    int roll = 0,
        p1score = 0,
        p2score = 0,
        p1loc = this->p1start,
        p2loc = this->p2start;
    bool p1next = true;
    for(; p1score < 1000 && p2score < 1000; p1next = !p1next){
        if(p1next){ move_deterministic(p1loc, roll, p1score); }
        else      { move_deterministic(p2loc, roll, p2score); }
    }
    return std::min(p1score, p2score) * roll;
}

typedef std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> state;
std::array<uint64_t, 2> TwentyOne::quantum(uint8_t p1loc, uint8_t p2loc, uint8_t p1score, uint8_t p2score){
    static std::map<state, std::array<uint64_t, 2> > memo;

    state S = {p1loc, p2loc, p1score, p2score};
    if(memo.find(S) != memo.end()){
        return memo[S];
    }
    else
    if(p1score >= 21){
        memo[S] = {1, 0};
        return memo[S];
    }
    else
    if(p2score >= 21){
        memo[S] = {0, 1};
        return memo[S];
    }

    uint64_t p1wins = 0, p2wins = 0;
    for(uint8_t i=0; i<27; ++i){
        auto [p1, p2, s1, s2] = S;
        p1 = (p1 + mq.at(i)) % 10;
        if(p1 == 0){ p1 = 10; }
        s1 += p1;
        std::array<uint64_t, 2> outcome = this->quantum(p2, p1, s2, s1);
        p1wins += outcome[1];
        p2wins += outcome[0];
    }
    memo[S] = {p1wins, p2wins};
    return memo[S];
}

int TwentyOne::part_one(){
    return this->deterministic();
}

uint64_t TwentyOne::part_two(){
    std::array<uint64_t, 2> wins = this->quantum(this->p1start, this->p2start, 0, 0);
    return std::max(wins[0], wins[1]);
}
