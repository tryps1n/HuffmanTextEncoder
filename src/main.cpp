#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include "fstream"
#include <unordered_map>
#include <vector>
#include "hufflib.cpp"
#include "iolib.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <flag> [files...]";
        cout << "Flags:" << endl;
        cout << "  -e <input> <output>  Encode/compress" << endl;
        cout << "  -d <input> <output>  Decode/decompress" << endl;
        return 1;
    }

    vector<string> arg;
    for(int i=1; i<argc; ++i)
    {
        arg.push_back((string)argv[i]); 
    }

    string path_to_in_file = filesystem::current_path().string() + "/" + arg[1];
    
    if (!filesystem::exists(path_to_in_file)) 
    {
        cout << "invalid file name" << endl;
        return 1;
    }

    string file_name(arg[1].begin(), arg[1].end()-4);
    file_name += ".huff";

    if (arg[0] == "-e")
    {
        ifstream infile(path_to_in_file);
        if (!infile.is_open())
        {
            cerr << "Error opening file" << endl;
            return 1;
        }
        string line, plain;
        while (getline(infile, line))
        {
            plain += line;
        }

        unordered_map<char, uint32_t> huff_freq = count_freq_string(plain);
        vector<Node*> nodes = nodes_from_freq(huff_freq);
        Node* root = build_Huffman_Tree(nodes);

        unordered_map<char, string> hufftable;
        build_Huffman_table(root, "0", hufftable);

        string encoded = Huffman_Encode(plain);
        
        fstream outfile(file_name, ios::binary | ios::out);

        write_bin_to_file(outfile, encoded, huff_freq);

        outfile.close();

        cout << "written successfully" << endl;
        
        // debug

        fstream binfile(file_name, ios::binary | ios::in);
        if (!binfile.is_open())
        {
            cerr << "Error opening file";
            return 1;
        }
        
        string decoded, bindata;
    }
    else if (arg[0] == "-d") {
        fstream binfile(arg[1], ios::binary | ios::in);
        if (!binfile.is_open())
        {
            cerr << "Error opening file" << endl;
            return 1;
        }
        unordered_map<char, uint32_t> frequency_map;
        string bin_from_file = read_bin_from_file(binfile, frequency_map);
        
        vector<Node*> nodes = nodes_from_freq(frequency_map);
        Node* huffmantree = build_Huffman_Tree(nodes);
        unordered_map<char, string> huff;
        build_Huffman_table(huffmantree, "0", huff);
        
        string plaintext = Huffman_Decode(bin_from_file, huff);
        cout << plaintext << endl;

        fstream plainoutfile(arg[1]+".txt", ios::out);
        
        plainoutfile.write(reinterpret_cast<char*>(&plaintext[0]), plaintext.size());
    }  
    else
    {   
        cout << "Wrong flag. Usage: " << endl;
        cout << "\"-e\" - encode" << endl;
        cout << "\"-d\" - decode" << endl; 
    }
    return 0;
}