#include "twenty.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

const std::unordered_map<char, bool> charmap{
    {'.', false},
    {'#', true}
};

const std::unordered_map<bool, char> mapchar{
    {false, '.'},
    {true, '#'}
};

Twenty::Twenty(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Twenty");
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

Twenty::~Twenty() = default;

void Twenty::load(){
    std::string x;
    std::getline(this->fin, x);
    for(int i=0; i<x.size(); ++i){ this->algo.push_back(charmap.at(x[i])); }

    std::getline(this->fin, x); // blank line b/t sections

    std::vector<bool> row;
    while(!this->fin.eof()){
        std::getline(this->fin, x);
        row.clear();
        for(int i=0; i<x.size(); ++i){ row.push_back(charmap.at(x[i])); }
        this->img.push_back(row);
    }
}

void Twenty::add_border(Twenty::Image& img, bool fill=false, int sz=2){
    for(int i=0; i<img.size(); ++i){
        for(int j=0; j<sz; ++j){
            img[i].insert(img[i].begin(), fill);
            img[i].push_back(fill);
        }
    }
    std::vector<bool> row(img[0].size(), fill);
    for(int j=0; j<sz; ++j){
        img.insert(img.begin(), row);
        img.push_back(row);
    }
}

void Twenty::trim_border(Twenty::Image& img, int sz=1){
    for(int i=0; i<sz; ++i){
        img.erase(img.begin());
        img.erase(img.end()-1);
    }
    for(int i=0; i<img.size(); ++i){
        for(int j=0; j<sz; ++j){
            img[i].erase(img[i].begin());
            img[i].erase(img[i].end()-1);
        }
    }
}

void Twenty::print_img(const Twenty::Image& img){
    for(int i=0; i<img.size(); ++i){
        for(int j=0; j<img[i].size(); ++j){
            std::cout << mapchar.at(img[i][j]);
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

Twenty::Image Twenty::enhance(const Twenty::Image& inp, int N){
    bool fill = false;
    Image res = inp;
    for(int n=0; n<N; ++n){
        this->add_border(res, fill, 2);
        Image img = res;
        const int X = img.size()-1,
                  Y = img[0].size()-1;

        for(int i=1; i<X; ++i){
            for(int j=1; j<Y; ++j){

                int idx = 0, k;
                for(int ii=-1; ii<2; ++ii){
                    for(int jj=-1; jj<2; ++jj){
                        if(res.at(i+ii).at(j+jj)){ idx |= 1 << (8 - 3*(ii+1) - jj - 1); }
                    }
                }
                img[i][j] = this->algo[idx];
            }
        }

        this->trim_border(img, 1);
        if(this->algo.front()){ fill = this->algo.back() ? true : ((n+1) % 2); }
        res = img;
    }
    return res;
}

int Twenty::count(const Twenty::Image& img){
    int res = 0;
    for(typename Image::const_iterator it=img.cbegin(); it!=img.cend(); ++it){
        for(std::vector<bool>::const_iterator jt=it->cbegin(); jt!=it->cend(); ++jt){
            if(*jt){ ++res; }
        }
    }
    return res;
}

int Twenty::part_one(){
    Image res = this->enhance(this->img, 2);
    return this->count(res);
}

int Twenty::part_two(){
    Image res = this->enhance(this->img, 50);
    return this->count(res);
}
