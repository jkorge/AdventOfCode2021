#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class Twenty{
public:
    typedef std::vector<std::vector<bool> > Image;
    typedef std::vector<bool> IEA;
private:
    std::ifstream fin;
    Image img;
    IEA algo;

    void load();
    void add_border(Image&, bool, int);
    void trim_border(Image&, int);
    void print_img(const Image&);
    Image enhance(const Image&, int);
    Image copy_img(const Image&);
    int count(const Image&);

public:
    Twenty(const std::string&);
    ~Twenty();

    int part_one();
    int part_two();
};

