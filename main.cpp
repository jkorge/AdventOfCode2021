#include "main.hpp"

int main(int argc, char** argv){

    std::cout.imbue(std::locale(std::cout.getloc(), new comma_sep));
    std::cout << std::boolalpha;

    // One("data/one.txt");
    // Two("data/two.txt");
    // Three("data/three.txt");
    // Four("data/four.txt");
    // Five("data/five.txt");
    // Six("data/six.txt");
    // Seven("data/seven.txt");
    // Eight("data/eight.txt");
    // Nine("data/nine.txt");
    // Ten("data/ten.txt");
    // Eleven("data/eleven.txt");
    // Twelve("data/twelve.txt");
    // Thirteen("data/thirteen.txt");
    // Fourteen("data/fourteen.txt");
    // Fifteen("data/fifteen.txt");
    // Sixteen("data/sixteen.txt");
    // Seventeen("data/seventeen.txt");
    // Eighteen("data/eighteen.txt");
    // Nineteen("data/nineteen.txt");
    // Twenty("data/twenty.txt");

    std::cout << "done." << std::endl;
    return 0;
}