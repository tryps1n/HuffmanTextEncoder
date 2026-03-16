#include "bits/stdc++.h"
#include <unordered_map>
#include <vector>

using namespace std;

struct Node {
    char c = '\0';
    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent = nullptr;
};

Node* addNodes(Node* a, Node* b){
    Node* c = new Node();
    c->value = a->value + b->value;

    if(a->value < b->value){
        c->left = a;
        c->right = b;
    }
    else {
        c->left = b;
        c->right = a;
    }
    return c;
}

void delete_tree(Node* node) {
    if (!node) return;
    delete_tree(node->left);
    delete_tree(node->right);
    delete node;
}

vector<Node*> nodes_from_freq(unordered_map<char, uint32_t>& dict){
    vector<Node*> nodes;
    for(const auto& x : dict){
        Node* node = new Node();
        node->c = x.first;
        node->value = x.second;
        nodes.push_back(node);
    }
    return nodes;
}

bool comp(Node* x, Node* y)
    {
        if (x->value = y->value) return x->c < y->c;
        else return x->value < y->value;
        
    }

Node* build_Huffman_Tree(vector<Node*> trees){
    if(trees.size() < 2) return trees[0];
    
    int n = trees.size();
    Node* a = nullptr;
    Node* b = nullptr;

    

    while(trees.size() > 1){

        sort(trees.begin(), trees.end(), comp);
        
        a = trees[0]; b = trees[1];
        trees.push_back(addNodes(a, b));

        trees.erase(trees.begin());
        trees.erase(trees.begin());

    }
    return trees[0];
}

void build_Huffman_table(Node* node, string code, unordered_map<char, string>& huff){
    if(!node) return; 

    if(node->c != '\0'){
        huff[node->c] = code;
        return;
    }

    build_Huffman_table(node->left, code+"1", huff);
    build_Huffman_table(node->right, code+"0", huff);
}

unordered_map<char, uint32_t> count_freq_string(string s){
    unordered_map<char, uint32_t> dict;
    for(char c : s){
        dict[c]++;
    }
    return dict;
}

string Huffman_Encode(string s){
    unordered_map<char, uint32_t> freq = count_freq_string(s);
    vector<Node*> nodes = nodes_from_freq(freq);

    Node* huff_tree = build_Huffman_Tree(nodes);

    unordered_map<char, string> huffcodes;
    build_Huffman_table(huff_tree, "", huffcodes);

    string huff_code;

    for(char c:s){
        huff_code += huffcodes[c];
    }

    delete_tree(huff_tree);
    return huff_code;
}

string Huffman_Decode(string coded, unordered_map<char, string> huff_map){
    string buffer = "";
    string output = "";

    unordered_map<string, char> reverse_huff_map;
    for(const auto& k : huff_map){
        reverse_huff_map[k.second] = k.first;
    }

    for (char c : coded){
        buffer += c;

        auto it = reverse_huff_map.find(buffer);
        if (it != reverse_huff_map.end()){
            output += it->second;
            buffer = "";
        }
    }
    return output;
}
