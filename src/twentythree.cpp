#include "twentythree.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

const std::set<int> rooms   = {3,4,7,8,11,12,15,16};
const std::set<int> rooms_a = {3,7,11,15};
const std::set<int> rooms_b = {4,8,12,16};
const std::set<int> hallway = {0,1,5,9,13,17,18};
const std::set<int> doorway = {2,6,10,14};
const std::map<char, int> targets = {
    {'A', 3},
    {'B', 7},
    {'C', 11},
    {'D', 15},
    {'.', -1}
};
const std::map<char, int> energies = {
    {'A', 1},
    {'B', 10},
    {'C', 100},
    {'D', 1000},
    {'.', -1},
};

typedef std::array<int, 2> move;

constexpr bool occupied(char c){
    switch(c){
        case '.': return false;
        default:  return true;
    }
}

int length(const std::string& state, int src, int dst){

    int pos = src,
        N = state.size(),
        res = 0;
    bool rightward = dst > src;

    while(pos != dst){
        if(rooms_a.count(pos)){ (dst == pos+1) ? ++pos : --pos; }
        else
        if(rooms_b.count(pos)){ --pos; }
        else
        if(doorway.count(pos)){ (dst == pos+1 || dst == pos+2) ? ++pos : (rightward ? (pos+=3) : --pos); }
        else
        if(pos<=1 || pos>=N-1){ rightward ? ++pos : --pos; }
        else
        if(hallway.count(pos)){ rightward ? ++pos : (pos-=3); }

        if(occupied(state.at(pos))){ return 0; }
        ++res;
    }
    return res;
}

int length(const std::string& state, move mv){ return length(state, mv[0], mv[1]); }

int cost(const std::string& state, move mv){
    return length(state, mv) * energies.at(state.at(mv[0]));
}

bool valid(const std::string& state, int src, int dst){
    /*
        1. src and dst are different
        2. dst is not a doorway
        3. clear path b/t src and dst
        4.
            a. src is room and dst is hallway
            b. dst is room_b and is the correct room for this amphipod
            c. dst is room_a, dst is the corect room for the amphipod, and room_b is appropriately occupied
    */

    char s = state.at(src), d = state.at(dst);
    return 
        src != dst &&
        !doorway.count(dst) &&
        length(state, src, dst) &&
        (
            (rooms.count(src) && hallway.count(dst)) ||
            (rooms_b.count(dst) && dst == targets.at(s)+1) ||
            (rooms_a.count(dst) && dst == targets.at(s) && state.at(dst+1) == s)
        );
}

std::vector<move> move_gen(const std::string& state){
    static std::unordered_map<std::string, std::vector<move> > memo;

    if(memo.find(state) != memo.end()){
        return memo[state];
    }

    std::vector<move> res;
    char amphipod;
    int N = state.size();
    for(int i=0; i<N; ++i){
        amphipod = state.at(i);
        if(!occupied(amphipod)){ continue; }

        // Don't move from correct room_b or from completed room
        if((i == targets.at(amphipod)+1) || (i == targets.at(amphipod) && state.at(i+1) == amphipod)){ continue; }

        for(int j=0; j<N; ++j){
            // Move to different, non-doorwary location, with clear path b/t
            if(valid(state, i, j)){ res.push_back({i, j}); }
        }
    }

    memo[state] = res;
    return res;
}


std::string make_move(const std::string& state, move mv){
    std::string res = state;
    res[mv[0]] = '.';
    res[mv[1]] = state[mv[0]];
    return res;
}

const std::string TARGET = "...AA..BB..CC..DD..";

int least_cost(const std::string& state, bool p=false){
    static int runs = 0;
    static std::unordered_map<std::string, int> memo = {
        {TARGET, 0}
    };
    ++runs;
    if(memo.find(state) != memo.end()){ return memo[state]; }

    int res = INT_MAX, c;
    std::vector<move> moves = move_gen(state);
    for(std::vector<move>::const_iterator it=moves.cbegin(); it!=moves.cend(); ++it){
        c = least_cost(make_move(state, *it));
        if(c < 0){ continue; }
        c += cost(state, *it);
        res = std::min(res, c);
    }

    if(res == INT_MAX){ res = -1; }
    memo[state] = res;
    return res;
}

TwentyThree::TwentyThree(const std::string& filename) : fin(filename), state(19, '.') {
    this->load();
    hdr::print("Day TwentyThree");
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

void TwentyThree::load(){
    std::string x;
    for(int i=0; i<2; ++i){ std::getline(this->fin, x); }

    std::getline(this->fin, x);
    std::set<int>::const_iterator it = rooms_a.begin();
    for(int i=0; i<x.size(); ++i){
        if(std::isalpha(x[i])){ this->state[*it++] = x[i]; }
    }

    std::getline(this->fin, x);
    it = rooms_b.begin();
    for(int i=0; i<x.size(); ++i){
        if(std::isalpha(x[i])){ this->state[*it++] = x[i]; }
    }

}

int TwentyThree::part_one(){
    return least_cost(this->state);
}

int TwentyThree::part_two(){
    return -1;
}
