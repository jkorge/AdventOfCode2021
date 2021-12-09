#include "nine.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;


Nine::Nine(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day Nine");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
    this->fin.close();
}

void Nine::load(){
    std::vector<int> row;
    char n;
    while(!fin.eof()){
        n = fin.get();
        if(std::isdigit(n)){ row.emplace_back(n - '0'); }
        else{ this->heightmap.emplace_back(row); row.clear(); }
    }
}

int Nine::basin_size(int i, int j, std::vector<std::vector<bool> >& basin_map){
    static int R = this->heightmap.size(),
               C = this->heightmap[0].size();

    // Start by counting this location and noting its height
    int res = 1, N = this->heightmap[i][j];

    // Mark this location as visited
    basin_map[i][j] = true;

    /*
        For each direction (up, down, left, right) check:
            1. Heightmap exists in that direction (ie. don't index @ -1 or >heightmap.size())
            2. Value in that direction is greater than current value
            3. Value in that direction is less than 9
            4. That location has not been visited yet

        If all four conditions pass, recurse with the new location's coordinates
    */
    if(i >   0 && N < this->heightmap[i-1][j  ] && this->heightmap[i-1][  j] != 9 && !basin_map[i-1][  j]){ res += basin_size(i-1,   j, basin_map); }
    if(i < R-1 && N < this->heightmap[i+1][j  ] && this->heightmap[i+1][  j] != 9 && !basin_map[i+1][  j]){ res += basin_size(i+1,   j, basin_map); }
    if(j >   0 && N < this->heightmap[i  ][j-1] && this->heightmap[  i][j-1] != 9 && !basin_map[  i][j-1]){ res += basin_size(  i, j-1, basin_map); }
    if(j < C-1 && N < this->heightmap[i  ][j+1] && this->heightmap[  i][j+1] != 9 && !basin_map[  i][j+1]){ res += basin_size(  i, j+1, basin_map); }

    return res;
}

int Nine::part_one(){
    int R = this->heightmap.size(),
        C = this->heightmap[0].size(),
        N;

    std::vector<int> lows;
    for(int i=0; i<R; ++i){
        for(int j=0; j<C; ++j){
            N = this->heightmap[i][j];
            if(
                (i ==   0 || N < this->heightmap[i-1][j  ]) &&
                (i == R-1 || N < this->heightmap[i+1][j  ]) &&
                (j ==   0 || N < this->heightmap[i  ][j-1]) &&
                (j == C-1 || N < this->heightmap[i  ][j+1])
            ){
                lows.emplace_back(N+1);
                this->heightmap.lowpoints.emplace_back(i,j);
            }
        }
    }

    int res = 0;
    for(int i=0; i<lows.size(); ++i){ res += lows[i]; }
    return res;
}

int Nine::part_two(){

    std::vector<int> sizes;
    for(std::vector<std::pair<int,int> >::iterator it=this->heightmap.lowpoints.begin(); it!=this->heightmap.lowpoints.end(); ++it){
        // 2D boolean vector for tracking visited coordinates
        std::vector<std::vector<bool> > basin_map(this->heightmap.size(), std::vector<bool>(this->heightmap[0].size(), false));
        sizes.emplace_back(this->basin_size(it->first, it->second, basin_map));
    }

    // Reverse sort & return product of 3 largest
    std::sort(sizes.rbegin(), sizes.rend());
    return sizes[0] * sizes[1] * sizes[2];
}