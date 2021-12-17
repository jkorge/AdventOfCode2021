#include "four.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

bool check_rows(const Board& b){

    for(int i=0; i<5; ++i){
        bool res = true;
        for(int j=0; j<5; ++j){ res &= b[i][j].marked; }
        if(res){ return true; }
    }
    return false;
}

bool check_cols(const Board& b){

    for(int j=0; j<5; ++j){
        bool res = true;
        for(int i=0; i<5; ++i){ res &= b[i][j].marked; }
        if(res){ return true; }
    }
    return false;
}

int score(const Board& b, int m){
    int res = 0;
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){ res += b[i][j].marked ? 0 : b[i][j].value; }
    }
    return res * m;
}

Four::Four(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Four");
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

void Four::load(){
    std::string x;
    std::getline(this->fin, x);
    std::istringstream str(x);
    while(std::getline(str, x, ',')){ this->moves.emplace_back(std::stoi(x)); }

    Board b;
    while(!this->fin.eof()){
        for(int i=0; i<5; ++i){
            for(int j=0; j<5; ++j){
                this->fin >> b[i][j].value;
                b[i][j].marked = false;
            }
        }
        this->boards.emplace_back(b);
    }
}

int Four::part_one(){

    for(std::vector<int>::const_iterator it=this->moves.cbegin(); it!=this->moves.cend(); ++it){
        for(std::vector<Board>::iterator bit=this->boards.begin(); bit!=this->boards.end(); ++bit){
            for(int i=0; i<5; ++i){
                for(int j=0; j<5; ++j){ (*bit)[i][j].marked |= (*bit)[i][j].value == *it; }
            }
            if(check_rows(*bit) || check_cols(*bit)){ return score(*bit, *it); }
        }
    }
    return -1;

}

int Four::part_two(){
    for(std::vector<int>::const_iterator it=this->moves.cbegin(); it!=this->moves.cend(); ++it){
        for(std::vector<Board>::iterator bit=this->boards.begin(); bit!=this->boards.end(); ){
            for(int i=0; i<5; ++i){
                for(int j=0; j<5; ++j){ (*bit)[i][j].marked |= (*bit)[i][j].value == *it; }
            }
            if(check_rows(*bit) || check_cols(*bit)){
                if(this->boards.size() == 1){ return score(*bit, *it); }
                else{ bit = this->boards.erase(bit); }
            }
            else{ ++bit; }
        }
    }
    return -1;
}

