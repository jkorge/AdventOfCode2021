#include "eleven.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

constexpr int N = 100;

int count_flashes(const flashmap& flash){
    int res = 0;
    for(int i=0; i<10; ++i){
        for(int j=0; j<10; ++j){
            if(flash[i][j]){ ++res; }
        }
    }
    return res;
}

void print_energy(const energymap& energy){
    for(int i=0; i<10; ++i){
        for(int j=0; j<10; ++j){
            std::cout << (int)energy[i][j];
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

Eleven::Eleven(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Eleven");
    tbl::header("Result", "Time");

    uint64_t t0 = Tempus::time();
    int x = this->part_one();
    uint64_t t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    this->load();

    t0 = Tempus::time();
    x = this->part_two();
    t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    tbl::sep();
    std::cout << std::endl;

    this->fin.close();
}

void Eleven::load(){
    this->fin.clear();
    this->fin.seekg(0);
    char x;
    for(int i=0; i<11 && !this->fin.eof(); ++i){
        for(int j=0; j<11 && !this->fin.eof(); ++j){
            this->fin.get(x);
            if(std::isdigit(x)){ this->energy[i][j] = x - '0'; }
        }
    }
}

void Eleven::one_for_all(){
    for(int i=0; i<10; ++i){
        for(int j=0; j<10; ++j){ ++this->energy[i][j]; }
    }
}

bool Eleven::all_marked(const flashmap& flash){
    for(int i=0; i<10; ++i){
        for(int j=0; j<10; ++j){
            if(this->energy[i][j] > 9 && !flash[i][j]){ return false; }
        }
    }
    return true;
}

void Eleven::xflash(int i, int j, flashmap& flash){

    if(this->energy[i][j] > 9 && !flash[i][j]){

        // Mark this location
        flash[i][j] = true;

        // Add to surrounding locations
        for(int x=i-1; x<i+2; ++x){
            for(int y=j-1; y<j+2; ++y){
                if(x == i && y == j){ continue; }
                else
                if((x >= 0 && x <=9) && (y >= 0 && y <=9)){ ++this->energy[x][y]; }
            }
        }

        // Recurse into surrounding locations
        for(int x=i-1; x<i+2; ++x){
            for(int y=j-1; y<j+2; ++y){
                if(x == i && y == j){ continue; }
                else
                if((x >= 0 && x <=9) && (y >= 0 && y <=9)){ this->xflash(x, y, flash); }
            }
        }
    }
}

void Eleven::reset(){
    for(int i=0; i<10; ++i){
        for(int j=0; j<10; ++j){
            if(this->energy[i][j] > 9){ this->energy[i][j] = 0; }
        }
    }
}

int Eleven::part_one(){
    int res = 0;

    for(int n=0; n<N; ++n){
        
        this->one_for_all();
        flashmap flash = {false};
        for(int i=0; i<10; ++i){ for(int j=0; j<10; ++j){ this->xflash(i, j, flash); } }
        res += count_flashes(flash);
        this->reset();

    }

    return res;
}

int Eleven::part_two(){
    bool all = false;
    for(int n=1; true; ++n){
        
        this->one_for_all();
        flashmap flash = {false};
        for(int i=0; i<10; ++i){ for(int j=0; j<10; ++j){ this->xflash(i, j, flash); } }
        this->reset();

        bool chk = true;
        for(int i=0; i<10; ++i){
            for(int j=0; j<10; ++j){
                chk &= flash[i][j];
            }
        }
        if(chk){ return n; }

    }

    return -1;
}
