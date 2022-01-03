#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class TwentyFour{

    typedef enum cmd{ inp, add, mul, div, mod, eql } cmd;

    struct Instruction{
        cmd op;
        char var;
        std::string val;
    };

    struct ALU{
        int w,x,y,z;

        void reset(){
            this->w = 0;
            this->x = 0;
            this->y = 0;
            this->z = 0;
        }

        std::string state(){
            return
                std::to_string(this->w) + " " +
                std::to_string(this->x) + " " +
                std::to_string(this->y) + " " +
                std::to_string(this->z);
        }

        int& sel(char var){
            switch(var){
                case 'x': return this->x;
                case 'y': return this->y;
                case 'z': return this->z;
                case 'w':
                default:  return this->w;
            }
        }

        void inp_(char var, int val){ this->sel(var)  = val; }
        void add_(char var, int val){ this->sel(var) += val; }
        void mul_(char var, int val){ this->sel(var) *= val; }
        void div_(char var, int val){ (val != 0) ? (this->sel(var) /= val) : 0; }
        void mod_(char var, int val){ (this->sel(var) >= 0 && val > 0) ? (this->sel(var) %= val) : 0; }
        void eql_(char var, int val){ this->sel(var) = (this->sel(var) == val) ? 1 : 0; }

        void run_(cmd op, char var, int val){
            switch(op){
                case inp: this->inp_(var, val); break;
                case add: this->add_(var, val); break;
                case mul: this->mul_(var, val); break;
                case div: this->div_(var, val); break;
                case mod: this->mod_(var, val); break;
                case eql:
                default:  this->eql_(var, val);
            }
        }

        void run(Instruction i){
            switch(i.val[0]){
                case 'x':
                case 'y':
                case 'z':
                case 'w': this->run_(i.op, i.var, this->sel(i.val[0])); break;
                default:  this->run_(i.op, i.var, std::stoi(i.val.data()));
            }
        }
    };

    std::ifstream fin;
    ALU alu;
    std::vector<Instruction> monad;

    void load();
    bool check_num(uint64_t);
    int64_t run(ALU, int);

public:
    TwentyFour(const std::string&);

    uint64_t part_one();
    int part_two();
};

