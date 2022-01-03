#include "five.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

Five::Five(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Five");
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

void Five::load(){
    coord ep1, ep2;
    char dmp[4];
    std::string line;
    int xlim = -1, ylim = -1;

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

        xlim = std::max(std::max(ep1.x, ep2.x), xlim);
        ylim = std::max(std::max(ep1.y, ep2.y), ylim);
        this->endpoints.emplace_back(ep1, ep2);
    }
    this->xlim = xlim+1;
    this->ylim = ylim+1;
}

int Five::part_one(){

    std::vector<std::vector<uint8_t> > counts(this->xlim, std::vector<uint8_t>(this->ylim, 0));

    for(std::vector<std::pair<coord, coord> >::iterator it=this->endpoints.begin(); it!=this->endpoints.end(); ++it){
        if(it->first.y == it->second.y){
            for(int x=std::min(it->first.x, it->second.x); x<=std::max(it->first.x, it->second.x); ++x){ ++counts[x][it->first.y]; }
        }
        else
        if(it->first.x == it->second.x){
            for(int y=std::min(it->first.y, it->second.y); y<=std::max(it->first.y, it->second.y); ++y){ ++counts[it->first.x][y]; }
        }
        else{ continue; }
    }
    int res = 0;
    for(std::vector<std::vector<uint8_t> >::iterator xit=counts.begin(); xit!=counts.end(); ++xit){
        for(std::vector<uint8_t>::iterator yit=xit->begin(); yit!=xit->end(); ++yit){
            if(*yit >=2){ ++res; }
        }
    }
    return res;
}

int Five::part_two(){

    std::vector<std::vector<uint8_t> > counts(this->xlim, std::vector<uint8_t>(this->ylim, 0));

    for(std::vector<std::pair<coord, coord> >::iterator it=this->endpoints.begin(); it!=this->endpoints.end(); ++it){
        if(it->first.y == it->second.y){
            for(int x=std::min(it->first.x, it->second.x); x<=std::max(it->first.x, it->second.x); ++x){ ++counts[x][it->first.y]; }
        }
        else
        if(it->first.x == it->second.x){
            for(int y=std::min(it->first.y, it->second.y); y<=std::max(it->first.y, it->second.y); ++y){ ++counts[it->first.x][y]; }
        }
        else{
            bool incx = it->first.x < it->second.x,
                 incy = it->first.y < it->second.y;

            if( incx &&  incy){ for(int x=it->first.x, y=it->first.y; x<=it->second.x && y<=it->second.y; ++x, ++y){ ++counts[x][y]; } }
            else
            if( incx && !incy){ for(int x=it->first.x, y=it->first.y; x<=it->second.x && y>=it->second.y; ++x, --y){ ++counts[x][y]; } }
            else
            if(!incx &&  incy){ for(int x=it->first.x, y=it->first.y; x>=it->second.x && y<=it->second.y; --x, ++y){ ++counts[x][y]; } }
            else
            if(!incx && !incy){ for(int x=it->first.x, y=it->first.y; x>=it->second.x && y>=it->second.y; --x, --y){ ++counts[x][y]; } }
        }
    }
    int res = 0;
    for(std::vector<std::vector<uint8_t> >::iterator xit=counts.begin(); xit!=counts.end(); ++xit){
        for(std::vector<uint8_t>::iterator yit=xit->begin(); yit!=xit->end(); ++yit){
            if(*yit >=2){ ++res; }
        }
    }
    return res;
}