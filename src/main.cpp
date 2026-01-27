#include <iostream>
#include <string>
#include <unordered_map>
#include "lib.h"

using namespace std;


int main(){

    string s;
    getline(cin, s);

    unordered_map<char, int> freq = count_freq_string(s);
    vector<Node*> nodes = nodes_from_freq(freq);
    Node* root = build_Huffman_Tree(nodes);

    unordered_map<char, string> hufftable;
    build_Huffman_table(root, "0", hufftable);

    string encoded = Huffman_Encode(s);
    
    cout << encoded << endl;

    cout << "decoded: " << Huffman_Decode(encoded, hufftable);

    return 0;
}