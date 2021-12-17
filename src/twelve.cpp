#include "twelve.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

// Empty path object
path empty;

void count_paths(Cave* c, int& count, path p=empty){
    p.push_back(c->name);
    if(c->name == "end"){
        ++count;
        return;
    }

    for(int i=0; i<c->tunnels.size(); ++i){
        if(c->tunnels[i]->name == "start"){ continue; }
        else
        if(c->tunnels[i]->small){
            bool visited = false;
            for(typename path::const_iterator it=p.cbegin(); it!=p.cend() && !visited; ++it){ visited = (*it) == c->tunnels[i]->name; }
            if(visited){ continue; }
        }
        count_paths(c->tunnels[i], count, p);
    }
}

void count_paths2(Cave* c, int& count, bool cycled=false, path p=empty){
    p.push_back(c->name);
    if(c->name == "end"){
        ++count;
        return;
    }

    for(int i=0; i<c->tunnels.size(); ++i){
        if(c->tunnels[i]->name == "start"){ continue; }
        
        bool visited = false;
        if(c->tunnels[i]->small){
            for(typename path::const_iterator it=p.cbegin(); it!=p.cend() && !visited; ++it){ visited = (*it) == c->tunnels[i]->name; }
            if(visited && cycled){ continue; }
        }
        count_paths2(c->tunnels[i], count, visited || cycled, p);
    }
}

Twelve::Twelve(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Twelve");
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

Twelve::~Twelve() = default;

void Twelve::load(){
    std::string x, n1, n2;
    Cave c1, c2;
    int idx;
    while(!this->fin.eof()){
        // Read next edge and separate cave names
        std::getline(this->fin, x);
        idx = x.find('-');
        n1 = x.substr(0, idx);
        n2 = x.substr(idx+1);

        // Check if caves are already constructed for the new names
        if(this->caves.find(n1) == this->caves.end()){ this->caves[n1] = new Cave(n1, std::islower(n1[0])); }
        if(this->caves.find(n2) == this->caves.end()){ this->caves[n2] = new Cave(n2, std::islower(n2[0])); }

        // Add caves to each other's list of connections
        this->caves[n1]->tunnels.push_back(this->caves[n2]);
        this->caves[n2]->tunnels.push_back(this->caves[n1]);
    }
}

int Twelve::part_one(){
    int res = 0;
    count_paths(&this->start, res);
    return res;
}

int Twelve::part_two(){
    int res = 0;
    count_paths2(&this->start, res);
    return res;
}
