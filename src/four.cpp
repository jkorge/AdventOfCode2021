#include "four.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;

bool check_rows(const Board& b){

    for(int i=0; i<5; ++i){
        bool res = true;
        for(int j=0; j<5; ++j){ res &= b[i][j].second; }
        if(res){ return true; }
    }
    return false;
}

bool check_cols(const Board& b){

    for(int j=0; j<5; ++j){
        bool res = true;
        for(int i=0; i<5; ++i){ res &= b[i][j].second; }
        if(res){ return true; }
    }
    return false;
}

int score(const Board& b, int m){
    int res = 0;
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){ res += b[i][j].second ? 0 : b[i][j].first; }
    }
    return res * m;
}

Four::Four(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day Four");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
    this->fin.close();
}

void Four::load(){
    std::string x;
    std::getline(this->fin, x);
    std::istringstream str(x);
    while(std::getline(str, x, ',')){ this->moves.emplace_back(std::stoi(x)); }

    Board b;
    int y;
    while(!this->fin.eof()){
        for(int i=0; i<5; ++i){
            for(int j=0; j<5; ++j){
                this->fin >> y;
                b[i][j] = std::make_pair(y, false);
            }
        }
        this->boards.emplace_back(b);
    }
}

int Four::part_one(){

    for(std::vector<int>::const_iterator it=this->moves.cbegin(); it!=this->moves.cend(); ++it){
        for(std::vector<Board>::iterator bit=this->boards.begin(); bit!=this->boards.end(); ++bit){
            for(int i=0; i<5; ++i){
                for(int j=0; j<5; ++j){ (*bit)[i][j].second |= (*bit)[i][j].first == *it; }
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
                for(int j=0; j<5; ++j){
                    (*bit)[i][j].second |= (*bit)[i][j].first == *it;
                }
            }
            bool wins = check_rows(*bit) || check_cols(*bit);
            if(wins){
                if(this->boards.size() == 1){ return score(*bit, *it); }
                else{ bit = this->boards.erase(bit); }
            }
            else{ ++bit; }
        }
    }
    return -1;
}

