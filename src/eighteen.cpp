#include "eighteen.hpp"
#include "table.hpp"
#include "tempus.hpp"

typedef table<2, 20, true> tbl;
typedef header<48> hdr;

Eighteen::Eighteen(const std::string& filename) : fin(filename) {
    this->load();
    hdr::print("Day Eighteen");
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

void Eighteen::load(){
    this->numbers.clear();
    this->fin.clear();
    this->fin.seekg(0);
    std::string x;
    while(!this->fin.eof()){
        std::getline(this->fin, x);
        Node* root = new Node;
        std::string::const_iterator it = x.cbegin(),
                                    eit = x.cend();
        this->make_tree(root, it, eit);
        this->numbers.push_back(root);
    }
}

void Eighteen::ptop(Eighteen::Node* root, std::string& str, std::string padding="", std::string pointer=""){
    static std::string fill = "|  ",
                       end = "\\--",
                       mid = "+--";

    if(root == nullptr){ return; }

    str += padding;
    str += pointer;
    str += (root->number == -1) ? "O" : std::to_string(root->number);
    str += '\n';
    this->ptop(root->right, str, padding + fill, end);
    this->ptop(root->left, str, padding + fill, root->right == nullptr ? mid : end);
}

void Eighteen::print_tree(Eighteen::Node* root){
    std::string t;
    this->ptop(root, t);
    std::cout << t << std::string(50, '-') << '\n';
}

void Eighteen::make_tree(Eighteen::Node* curr, std::string::const_iterator& it, std::string::const_iterator& eit){
    
    if(it == eit){ return; }
    char x;

    while(it != eit){
        x = *it++;
        switch(x){
            case '[': {
                curr->left = new Node;
                curr->left->parent = curr;
                make_tree(curr->left, it, eit);
                break;
            }
            case ']': {
                return;
            }
            case ',': {
                curr->right = new Node;
                curr->right->parent = curr;
                make_tree(curr->right, it, eit);
                break;
            }
            default: {
                curr->number = x - '0';
                return;
            }
        }
    }
}

void Eighteen::explode_backup(Eighteen::Node* n, int x, bool left){

    // Reached root of tree, nowhere left to go, discard number
    if(n->parent == nullptr){ return; }

    Node* p = left ? n->parent->left : n->parent->right;

    if(p == n)         { this->explode_backup(n->parent, x, left); }
    else
    if(p->number == -1){ this->explode_descend(p, x, !left); }
    else               { p->number += x; }
}

void Eighteen::explode_descend(Eighteen::Node* n, int x, bool left){

    if(n == nullptr)   { return; }
    else
    if(n->number == -1){ this->explode_descend(left ? n->left : n->right, x, left); }
    else               { n->number += x; }
}

bool Eighteen::explode_search(Eighteen::Node* root, int lvl=0){
    if(root == nullptr){ return true; }
    else
    if(lvl == 4 && root->number == -1){
        // explode
        this->explode_backup(root, root->left->number, true);
        this->explode_backup(root, root->right->number, false);
        delete root->left;
        delete root->right;
        root->left = nullptr;
        root->right = nullptr;
        root->number = 0;
        return false;
    }
    else
    if(!this->explode_search(root->left, lvl+1) || !this->explode_search(root->right, lvl+1)){ return false; }
    return true;
}

bool Eighteen::split_search(Eighteen::Node* root, int lvl=0){
    if(root == nullptr){ return true; }
    else
    if(root->number >= 10){
        // split
        int left_number = root->number / 2,
            right_number = std::ceil(float(root->number) / 2.0);
        root->left = new Node(left_number);
        root->right = new Node(right_number);
        root->number = -1;
        root->left->parent = root;
        root->right->parent = root;
        return false;
    }
    else
    if(!this->split_search(root->left, lvl+1) || !this->split_search(root->right, lvl+1)){ return false; }
    return true;
}

void Eighteen::reduce(Eighteen::Node* root){
    while(true){
        if(!this->explode_search(root)){ continue; }
        if(!this->split_search(root))  { continue; }
        break;
    }
}

int Eighteen::magnitude(Eighteen::Node* root){
    int res = 0;
    if(root == nullptr){ res = 0; }
    else
    if(root->number == -1){
        res += 3*this->magnitude(root->left);
        res += 2*this->magnitude(root->right);
    }
    else{ res = root->number; }

    return res;
}

Eighteen::Node* Eighteen::add(Eighteen::Node* n1, Eighteen::Node* n2){
    Node* res = new Node;
    res->left = n1;
    res->right = n2;
    n1->parent = res;
    n2->parent = res;
    return res;
}

void Eighteen::copy_tree(Eighteen::Node* root, Eighteen::Node* copy){

    if(root == nullptr){ return; }

    copy->number = root->number;
    if(root->left != nullptr){
        copy->left = new Node;
        copy->left->parent = copy;
    }
    if(root->right != nullptr){
        copy->right = new Node;
        copy->right->parent = copy;
    }

    copy_tree(root->left, copy->left);
    copy_tree(root->right, copy->right);
}

int Eighteen::part_one(){

    std::vector<Node*> copies;
    for(int i=0; i<this->numbers.size(); ++i){
        Node* n = new Node;
        this->copy_tree(this->numbers[i], n);
        copies.push_back(n);
    }

    Node* sum = copies[0];
    for(int i=1; i<copies.size(); ++i){
        sum = this->add(sum, copies[i]);
        this->reduce(sum);
    }
    return this->magnitude(sum);
}

int Eighteen::part_two(){
    int res = INT_MIN;

    for(int i=0; i<this->numbers.size(); ++i){
        for(int j=0; j<this->numbers.size(); ++j){
            if(i == j){ continue; }
            Node* a = new Node;
            Node* b = new Node;
            this->copy_tree(this->numbers[i], a);
            this->copy_tree(this->numbers[j], b);
            Node* n = this->add(a, b);
            this->reduce(n);
            res = std::max(res, this->magnitude(n));
        }
    }

    return res;
}
