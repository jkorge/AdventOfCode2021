#include "ten.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 35, true> tbl;
typedef header<78> hdr;

const std::unordered_map<char, char> complements{
    {'[', ']'},
    {'{', '}'},
    {'(', ')'},
    {'<', '>'}
};

const std::unordered_map<char, int> corrupted_scores{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
};

const std::unordered_map<char, int> completed_scores{
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4}
};

Ten::Ten(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Ten");
    tbl::header("Result", "Time");

    uint64_t t0 = Tempus::time();
    int x = this->part_one();
    uint64_t t1 = Tempus::time();
    tbl::row(x, Tempus::strtime(t1-t0));

    t0 = Tempus::time();
    long long y = this->part_two();
    t1 = Tempus::time();
    tbl::row(y, Tempus::strtime(t1-t0));

    tbl::sep();
    std::cout << std::endl;

    this->fin.close();
}

void Ten::load(){
    std::string x;
    while(!this->fin.eof()){
        std::getline(this->fin, x);
        this->data.emplace_back(x);
    }
}

int Ten::part_one(){
    int res = 0;

    for(int i=0; i<this->data.size(); ++i){

        bool corrupt = false;
        std::stack<char> expected; // Expected sequence of closing chars

        for(std::string::const_iterator it=this->data[i].cbegin(); it!=this->data[i].cend() && !corrupt; ++it){
            switch(*it){
                case '[':
                case '{':
                case '(':
                case '<': expected.push(complements.at(*it)); break;
                default: {
                    // Accumulate scores if corrupted
                    if(corrupt = *it != expected.top()){ res += corrupted_scores.at(*it); }
                    expected.pop();
                }
            }
        }
    }

    return res;
}

long long Ten::part_two(){
    std::vector<long long> scores;

    for(int i=0; i<this->data.size(); ++i){

        bool corrupt = false;
        long long score = 0;
        std::stack<char> expected; // Expected sequence of closing chars

        for(std::string::const_iterator it=this->data[i].cbegin(); it!=this->data[i].cend() && !corrupt; ++it){
            switch(*it){
                case '[':
                case '{':
                case '(':
                case '<': expected.push(complements.at(*it)); break;
                default: corrupt = *it != expected.top(); expected.pop();
            }
        }

        // Only score uncorrupted lines
        if(!corrupt){
            while(!expected.empty()){
                score *= 5;
                score += completed_scores.at(expected.top());
                expected.pop();
            }
            scores.emplace_back(score);
        }
    }

    // Return median score
    std::sort(scores.begin(), scores.end());
    return scores.at(scores.size() / 2);
}