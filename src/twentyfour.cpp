#include "twentyfour.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

TwentyFour::TwentyFour(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day TwentyFour");
    tbl::header("Result", "Time");

    uint64_t t0 = Tempus::time();
    uint64_t x = this->part_one();
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

void TwentyFour::load(){
    std::string x, val;
    cmd op;
    char var;
    while(!this->fin.eof()){
        std::getline(this->fin, x);
        var = x[4];
        val = (x.size() > 6) ? x.substr(6) : "0";
        switch(x[0]){
            case 'i': op = inp; break;
            case 'a': op = add; break;
            case 'd': op = div; break;
            case 'e': op = eql; break;
            case 'm':
            default:  op = x[1]=='u' ? mul : mod;
        }
        this->monad.push_back({op, var, val});
    }
}

bool TwentyFour::check_num(uint64_t N){
    std::string x = std::to_string(N);
    if((x.size() < 14) || (x.find('0') != std::string::npos)){ return false; }
    int i = 0;
    this->alu.reset();
    for(std::vector<Instruction>::iterator it=this->monad.begin(); it!=this->monad.end(); ++it){
        if(it->op == inp){ it->val = x.substr(i++, 1); }
        this->alu.run(*it);
    }
    return this->alu.z == 0;
}

int64_t TwentyFour::run(TwentyFour::ALU a, int N){

    Instruction ins;
    int64_t lower;
    ALU bak = a;
    for(int n=9; n>0; --n){
        a = bak;
        for(int i=0; i<18; ++i){
            ins = this->monad[i];
            if(ins.op == inp){ ins.val = std::to_string(n); }
            a.run(ins);
        }
        if(N > 0){
            lower = this->run(a, N-1);
            if(lower > 0){
                int64_t p = std::to_string(lower).size();
                int64_t upper = n * std::pow(10, p);
                return upper + lower;
            }
        }
        else
        if(a.z == 0){
            return n;
        }
    }

    return -1;
}

uint64_t TwentyFour::part_one(){
    ALU a;
    return this->run(a, 13);
}

int TwentyFour::part_two(){
    return 0;
}
