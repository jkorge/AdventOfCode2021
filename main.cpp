#include "main.hpp"

int main(int argc, char** argv){

    std::cout.imbue(std::locale(std::cout.getloc(), new comma_sep));

    // One("data/one.txt");
    // Two("data/two.txt");
    // Three("data/three.txt");
    // Four("data/four.txt");
    // Five("data/five.txt");
    // Six("data/six.txt");
    // Seven("data/seven.txt");
    // Eight("data/eight.txt");
    Nine("data/nine.txt");

    return 0;
}