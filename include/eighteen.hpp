#include <array>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Eighteen{

public:
    struct Node{
        int number{-1};
        Node* left = nullptr;
        Node* right = nullptr;
        Node* parent = nullptr;

        Node() {}
        Node(int x) : number(x) {}
    };

private:
    std::ifstream fin;
    std::vector<Node*> numbers;

    void load();
    void ptop(Node*, std::string&, std::string, std::string);
    void print_tree(Node*);
    void make_tree(Node*, std::string::const_iterator&, std::string::const_iterator&);
    void explode_backup(Node*, int, bool);
    void explode_descend(Node*, int, bool);
    bool explode_search(Node*, int);
    bool split_search(Node*, int);
    void reduce(Node* root);
    int magnitude(Node*);
    Node* add(Node*, Node*);
    void copy_tree(Node*, Node*);

public:
    Eighteen(const std::string&);

    int part_one();
    int part_two();
};

