#include "thirteen.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

Thirteen::Thirteen(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Thirteen");
    tbl::header("Result", "Time");

    uint64_t t0 = Tempus::time();
    int x = this->part_one();
    uint64_t t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    t0 = Tempus::time();
    std::string y = this->part_two();
    t1 = Tempus::time();
    tbl::row(y, Tempus::strtime(t1-t0));

    tbl::sep();
    std::cout << std::endl;

    this->fin.close();
}

Thirteen::~Thirteen() = default;

void Thirteen::load(){
    std::string x;
    bool section = false;
    int idx, cx, cy;
    while(!this->fin.eof()){

        std::getline(this->fin, x);
        if(x.empty()){ section = true; continue; }
        else
        if(section){
            idx = x.rfind(' ');
            x = x.substr(idx+1);
            this->ins.folds.emplace_back(x[0] == 'y', std::stoi(x.substr(2).data()));
        }
        else{
            idx = x.find(',');
            cx = std::stoi(x.substr(0,idx).data());
            cy = std::stoi(x.substr(idx+1).data());
            this->ins.coords.emplace_back(cx, cy);
        }
    }
}

std::vector<Thirteen::coord> horizontal_fold(const int y, const std::vector<Thirteen::coord>& coords){
    std::vector<Thirteen::coord> res;
    for(int i=0; i<coords.size(); ++i){
        if(coords.at(i).y < y){ res.emplace_back(coords.at(i)); }
        else{
            res.emplace_back(coords.at(i).x, y - (coords.at(i).y - y));
        }
    }
    return res;
}

std::vector<Thirteen::coord> vertical_fold(const int x, const std::vector<Thirteen::coord>& coords){
    std::vector<Thirteen::coord> res;
    for(int i=0; i<coords.size(); ++i){
        if(coords.at(i).x < x){ res.emplace_back(coords.at(i)); }
        else{
            res.emplace_back(x - (coords.at(i).x - x), coords.at(i).y);
        }
    }
    return res;
}

std::vector<Thirteen::coord> fold_coords(Thirteen::fold& f, const std::vector<Thirteen::coord>& coords){
    return f.first ? horizontal_fold(f.second, coords) : vertical_fold(f.second, coords);
}

int Thirteen::part_one(){

    // Map coords to their new locations
    std::vector<coord> new_coords = fold_coords(this->ins.folds[0], this->ins.coords);

    // Determine size of folded paper
    int xlim = 0, ylim = 0;
    for(int i=0; i<new_coords.size(); ++i){
        xlim = std::max(xlim, new_coords[i].x);
        ylim = std::max(ylim, new_coords[i].y);
    }

    // Allocate vector for folded paper and mark where dots appear
    grid folded_paper = std::vector<std::vector<bool> >(ylim+1, std::vector<bool>(xlim+1, false));
    for(int i=0; i<new_coords.size(); ++i){
        folded_paper[new_coords[i].y][new_coords[i].x] = true;
    }

    // Count marks
    int res = 0;
    for(int i=0; i<folded_paper.size(); ++i){
        for(int j=0; j<folded_paper[i].size(); ++j){
            if(folded_paper[i][j]){ ++res; }
        }
    }
    return res;
}

std::string Thirteen::part_two(){
    std::vector<coord> new_coords = this->ins.coords;

    for(int i=0; i<this->ins.folds.size(); ++i){
        // Map coords to their new locations
        new_coords = fold_coords(this->ins.folds[i], new_coords);
    }

    // Determine size of folded paper
    int xlim = 0, ylim = 0;
    for(int i=0; i<new_coords.size(); ++i){
        xlim = std::max(xlim, new_coords[i].x);
        ylim = std::max(ylim, new_coords[i].y);
    }

    // Allocate vector for folded paper and mark where dots appear
    grid folded_paper = std::vector<std::vector<bool> >(ylim+1, std::vector<bool>(xlim+1, false));
    for(int i=0; i<new_coords.size(); ++i){
        folded_paper[new_coords[i].y][new_coords[i].x] = true;
    }

    std::string res = "\n";
    int N = folded_paper.size();
    for(int i=0; i<N; ++i){
        for(int j=0; j<folded_paper[i].size(); ++j){
            res += (folded_paper[i][j] ? '#' : ' ');
        }
        if(i <N-1){ res += '\n'; }
        else      { res += "  "; }
    }
    return res;
}
