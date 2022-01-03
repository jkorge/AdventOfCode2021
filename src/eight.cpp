#include "eight.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

std::basic_istream<char>& operator>>(std::basic_istream<char>& is, Record& r){
    std::string signals, output, signal, out;

    std::getline(is, signals, '|');
    std::istringstream str(signals);

    for(int i=0; !str.eof(); ++i){
        str >> signal;
        r.signals[i] = signal;
    }

    is.get(); // space
    std::getline(is, output);
    str.clear();
    str.str(output);

    for(int i=0; !str.eof(); ++i){
        str >> out;
        r.output[i] = out;
    }

    return is;
}

// Check if map `m` contains value `val`
template<typename T1, typename T2>
bool hasvalue(const std::unordered_map<T1, T2>& m, T2 val){
    for(auto it=m.cbegin(); it!=m.cend(); ++it){
        if(val == it->second){ return true; }
    }
    return false;
}

std::unordered_map<char, std::string> Record::decode() const{
    std::unordered_map<char, std::string> mixed;

    // Find 1, 4, 7, and 8
    for(int i=0; i<10; ++i){
        switch(this->signals.at(i).size()){
            case 2: mixed['1'] = this->signals.at(i); break;
            case 3: mixed['7'] = this->signals.at(i); break;
            case 4: mixed['4'] = this->signals.at(i); break;
            case 7: mixed['8'] = this->signals.at(i); break;
            default: continue;
        }
    }

    /*
        Figure out 3 and 6

        Three digits are represented by five segments: 2, 3, and 5.
        Of these, only 3 has two segments in common with 1; 2 and 5 each only have one segment in common with 1.

        Three digits are represented by six segments: 0, 6, and 9.
        Of these, only 6 has one segment in common with 1; 0 and 9 each have two segments in common with 1.
    */
    for(int i=0; i<10; ++i){
        if(hasvalue(mixed, this->signals.at(i))){ continue; }

        switch(this->signals.at(i).size()){

            case 5: {
                int same = 0;
                for(int j=0; j<5; ++j){
                    if(mixed['1'].find(this->signals.at(i)[j]) != std::string::npos){ ++same; }
                }
                if(same == 2){ mixed['3'] = this->signals.at(i); }
                break;
            }

            case 6: {
                int same = 0;
                for(int j=0; j<6; ++j){
                    if(mixed['1'].find(this->signals.at(i)[j]) != std::string::npos){ ++same; }
                }
                if(same == 1){ mixed['6'] = this->signals.at(i); }
                break;
            }

            default: continue;
        }
    }

    /*
        Figure out 5, 2, 0, and 9

        Of the two remaining five-segment digits (2 and 5), 5 contains only one segment not found in 6; 2 contains more.
        Of the two remaining six-segment digits (0 and 9), 9 contains only one segment not found in 3; 0 contains more.
    */
    for(int i=0; i<10; ++i){
        if(hasvalue(mixed, this->signals.at(i))){ continue; }

        switch(this->signals.at(i).size()){

            case 5: {
                int diff = 0;
                for(int j=0; j<mixed['6'].size(); ++j){
                    if(this->signals.at(i).find(mixed['6'][j]) == std::string::npos){ ++diff; }
                }
                mixed[diff == 1 ? '5' : '2'] = this->signals.at(i);
                break;
            }

            case 6: {
                int diff = 0;
                for(int j=0; j<6; ++j){
                    if(mixed['3'].find(this->signals.at(i)[j]) == std::string::npos){ ++diff; }
                }
                mixed[diff == 1 ? '9' : '0'] = this->signals.at(i);
                break;
            }

            default: continue;
        }
    }

    return mixed;
}

Eight::Eight(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Eight");
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

void Eight::load(){
    Record r;
    int i = 0;
    while(!this->fin.eof()){
        this->fin >> r;
        this->records.emplace_back(r);
    }
}

int Eight::part_one(){
    int res = 0;
    for(std::vector<Record>::const_iterator it=this->records.cbegin(); it!=this->records.cend(); ++it){
        for(int j=0; j<4; ++j){
            switch(it->output[j].size()){
                case 2:
                case 3:
                case 4:
                case 7: ++res; break;
                default: continue;
            }
        }
    }
    return res;
}

int Eight::part_two(){

    int res = 0;

    for(std::vector<Record>::iterator it=this->records.begin(); it!=this->records.end(); ++it){

        std::unordered_map<char, std::string> mixed = it->decode();

        // Sort strings and reverse map
        std::unordered_map<std::string, char> dexim;
        for(std::unordered_map<char, std::string>::iterator mit=mixed.begin(); mit!=mixed.end(); ++mit){
            std::sort(mit->second.begin(), mit->second.end());
            dexim[mit->second] = mit->first;
        }

        std::string number;
        for(int i=0; i<4; ++i){
            std::sort(it->output[i].begin(), it->output[i].end());
            number += dexim[it->output[i]];
        }
        res += std::stoi(number);
    }
    return res;
}

