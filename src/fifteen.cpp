#include "fifteen.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

Fifteen::Fifteen(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Fifteen");
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

void Fifteen::load(){
    char x;
    std::vector<uint8_t> row;
    this->fin.get(x);
    while(!this->fin.eof()){
        switch(x){
            case '\n': this->grid.emplace_back(row); row.clear(); break;
            default: row.emplace_back(x - '0');
        }
        this->fin.get(x);
    }
    this->grid.emplace_back(row);
}

int dijkstra(const std::vector<std::vector<uint8_t> >& grid){
    int X = grid.size(), Y = grid.at(0).size();

    std::vector<std::vector<bool> > spnodes(X, std::vector<bool>(Y, false));
    std::vector<std::vector<int> > distances(X, std::vector<int>(Y, INT_MAX));
    distances[0][0] = 0;

    int p = (X*Y / 100);

    for(int n=0; n<X*Y; ++n){

        if(!(n%p)){ std::cout << (n/p) << "%\r"; }

        // Find unvisited node with min distance
        int x, y, min = INT_MAX;
        std::pair<int, int> idx;
        for(int i=0; i<X; ++i){
            for(int j=0; j<Y; ++j){
                if(!spnodes[i][j] && distances[i][j] <= min){
                    min = distances[i][j];
                    x = i;
                    y = j;
                }
            }
        }

        // Mark this node as visited
        spnodes[x][y] = true;

        if(
            x > 0 &&
            !spnodes[x-1][y] &&
            distances[x][y] != INT_MAX &&
            (distances[x][y] + grid.at(x-1)[y] < distances[x-1][y])
        ){ distances[x-1][y] = distances[x][y] + grid.at(x-1)[y]; }
        if(
            x < X-1 &&
            !spnodes[x+1][y] &&
            distances[x][y] != INT_MAX &&
            (distances[x][y] + grid.at(x+1)[y] < distances[x+1][y])
        ){ distances[x+1][y] = distances[x][y] + grid.at(x+1)[y]; }
        if(
            y > 0 &&
            !spnodes[x][y-1] &&
            distances[x][y] != INT_MAX &&
            (distances[x][y] + grid.at(x)[y-1] < distances[x][y-1])
        ){ distances[x][y-1] = distances[x][y] + grid.at(x)[y-1]; }
        if(
            y < Y-1 &&
            !spnodes[x][y+1] &&
            distances[x][y] != INT_MAX &&
            (distances[x][y] + grid.at(x)[y+1] < distances[x][y+1])
        ){ distances[x][y+1] = distances[x][y] + grid.at(x)[y+1]; }
    }

    return distances[X-1][Y-1];
}

int Fifteen::part_one(){ return dijkstra(this->grid); }

int Fifteen::part_two(){

    const int X = this->grid.size(), Y = this->grid[0].size();
    const int NX = 5*X, NY = 5*Y;

    std::vector<std::vector<uint8_t> > full(NX, std::vector<uint8_t>(NY, 0));

    for(int x=0; x<5; ++x){
        for(int y=0; y<5; ++y){

            for(int i=0; i<X; ++i){
                for(int j=0; j<Y; ++j){
                    int nx = i + (x*X),
                        ny = j + (y*Y),
                        nv = this->grid[i][j] + x + y;
                    while(nv > 9){ nv -= 9; }
                    full[nx][ny] = nv > 9 ? 1 : nv;
                }
            }

        }
    }

    return dijkstra(full);
}
