#include "five.hpp"
#include "table.hpp"

typedef table<1, 20, true> tbl;

Five::Five(const std::string& filename) : fin(filename) {
    this->load();
    tbl::header("Day Five");
    tbl::row(this->part_one());
    tbl::row(this->part_two());
    tbl::sep();
    this->fin.close();
}

void Five::load(){
    coord ep1, ep2;
    char dmp[4];
    std::string line;

    while(!fin.eof()){
        // read line
        std::getline(fin, line);
        std::istringstream str(line);

        // first endpoint
        str >> ep1.x;
        str.get();
        str >> ep1.y;

        // arrow characters
        str.read(dmp, 4);

        // second endpoint
        str >> ep2.x;
        str.get();
        str >> ep2.y;

        this->endpoints.emplace_back(ep1, ep2);
    }


}

int Five::part_one(){
    std::unordered_map<coord, int, CoordHash> counts;

    for(std::vector<std::pair<coord, coord> >::const_iterator it=this->endpoints.cbegin(); it!=this->endpoints.cend(); ++it){
        // Only consider horizontal/vertical lines
        bool horizontal = it->first.y == it->second.y,
             vertical = it->first.x == it->second.x;
        if(!horizontal && !vertical){ continue; }

        bool increasing = horizontal ? it->first.x < it->second.x : it->first.y < it->second.y;
        for(coord c=it->first;
            horizontal ? (increasing ? c.x <= it->second.x : c.x >= it->second.x)
                       : (increasing ? c.y <= it->second.y : c.y >= it->second.y);
            horizontal ? (increasing ? ++c.x : --c.x)
                       : (increasing ? ++c.y : --c.y)
        ){
            ++counts[c];
        }
    }

    int res = 0;
    for(std::unordered_map<coord, int>::iterator it=counts.begin(); it!=counts.end(); ++it){ if(it->second >= 2){ ++res; } }
    return res;
}

int Five::part_two(){
    std::unordered_map<coord, int, CoordHash> counts;

    for(std::vector<std::pair<coord, coord> >::const_iterator it=this->endpoints.cbegin(); it!=this->endpoints.cend(); ++it){
        bool horizontal = it->first.y == it->second.y,
             vertical = it->first.x == it->second.x,
             incx = it->first.x < it->second.x,
             incy = it->first.y < it->second.y;

        coord c = it->first;
        while(true){
            ++counts[c];

            if(horizontal){
                incx ? ++c.x : --c.x;
                if(incx ? c.x > it->second.x : c.x < it->second.x){ break; }
            }
            else
            if(vertical){
                incy ? ++c.y : --c.y;
                if(incy ? c.y > it->second.y : c.y < it->second.y){ break; }
            }
            else{
                incx ? ++c.x : --c.x;
                incy ? ++c.y : --c.y;
                if((incx ? c.x > it->second.x : c.x < it->second.x) || (incy ? c.y > it->second.y : c.y < it->second.y)){ break; }
            }
        }
    }

    int res = 0;
    for(std::unordered_map<coord, int>::iterator it=counts.begin(); it!=counts.end(); ++it){ if(it->second >= 2){ ++res; } }
    return res;
}