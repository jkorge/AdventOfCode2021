#include "two.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;

Two::Two(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day Two");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
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
            case down: this->position[1] += it->second; break;
            default: this->position[1] -= it->second;
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
            default: this->position[2] -= it->second;
        }
    }
    return this->position[0] * this->position[1];
}