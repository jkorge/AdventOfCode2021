#include "two.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

Two::Two(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Two");
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

void Two::load(){
    std::string cmd;
    int x;
    while(!this->fin.eof()){
        fin >> cmd;
        fin >> x;
        if(cmd == "forward"){ this->data.emplace_back(std::make_pair(forward, x)); }
        else
        if(cmd == "down")   { this->data.emplace_back(std::make_pair(down, x));    }
        else
        if(cmd == "up")     { this->data.emplace_back(std::make_pair(up, x));      }
    }
}

int Two::part_one(){
    this->position.fill(0);
    for(std::vector<std::pair<direction, int> >::iterator it=this->data.begin(); it!=this->data.end(); ++it){
        switch(it->first){
            case forward: this->position[0] += it->second; break;
            case down:    this->position[1] += it->second; break;
            default:      this->position[1] -= it->second; // case up
        }
    }
    return this->position[0] * this->position[1];
}

int Two::part_two(){
    this->position.fill(0);
    for(std::vector<std::pair<direction, int> >::iterator it=this->data.begin(); it!=this->data.end(); ++it){
        switch(it->first){
            case forward: {
                this->position[0] += it->second;
                this->position[1] += this->position[2] * it->second;
                break;
            }
            case down: this->position[2] += it->second; break;
            default:   this->position[2] -= it->second; // case up
        }
    }
    return this->position[0] * this->position[1];
}