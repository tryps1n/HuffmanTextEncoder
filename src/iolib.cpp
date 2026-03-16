#include "bits/stdc++.h"
#include "fstream"
#include <cstdint>
#include <fstream>
#include <ostream>
#include <string>
#include <unordered_map>

using namespace std;

#pragma pack(push, 1)
struct Header 
{
    unsigned char magic_number[4] = {'H', 'U', 'F', 'F'};
    uint32_t freq_table_entries;
    uint32_t bit_number;
    uint8_t padding_size;
};
#pragma pack(pop)

void write_bin_to_file(fstream& file, string bits, unordered_map<char, uint32_t>& freq_table)
{
    Header header;
    header.bit_number = bits.size();
    header.freq_table_entries = freq_table.size();
    header.padding_size = (8 - (bits.size() % 8)) % 8;
    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    //writes header class into file. 

    for (const auto& p : freq_table)
    {
        file.write(reinterpret_cast<const char*>(&p.first), 1);
        file.write(reinterpret_cast<const char*>(&p.second), 4);
    }
    //writes unordered_map data

    unsigned char current_byte = 0; int bit_position = 0;
    // create empty byte using unsigned char, which holds one byte
    
    for (char c : bits)
    {
        current_byte = (current_byte << 1) | (c - '0'); 
        // shift current_byte by 1 to make room for new bit
        // bitwise OR -> add one byte to the newly created 0 at the right
        // (c-'0') gives binary for either 1 or 0, since c can be either '1' or '0'
        bit_position++;

        if (bit_position == 8)
        {
            file.write(reinterpret_cast<char*>(&current_byte), 1);
            // write the byte into the bin file by casting unsigned char pointer into a
            // char pointer pointing to the address, write 1 bit starting from address
            current_byte = 0; bit_position = 0;
        }
    }
    if (bit_position != 0)
    {
        current_byte <<= 8 - bit_position;
        file.write(reinterpret_cast<char*>(&current_byte), 1);
    }
}

string read_bin_from_file(fstream& file, unordered_map<char, uint32_t>& frequency_table)
{
    file.seekg(0, ios::beg);
    
    Header header;
    if (!file.read(reinterpret_cast<char*>(&header), sizeof(Header)))
    {
        cout << "cannot read file header." << endl;
        return "";
    }
    // read bytes of size Header class and put into the memory address of header
    
    string magic(reinterpret_cast<const char*>(header.magic_number), 4);
    if (magic != "HUFF")
    {
        return "\0";
    }   
    uint32_t freqnumber = header.freq_table_entries;
    uint32_t bitnumber = header.bit_number;
    uint8_t padding = header.padding_size;
    // read a total of 5 * freqnumber bytes

    char ch; uint32_t f;
    for (uint32_t i=0; i<freqnumber; ++i)
    {
        file.read(reinterpret_cast<char*>(&ch), 1);
        file.read(reinterpret_cast<char*>(&f), 4);

        frequency_table[ch] = f;
    }

    string output = "";
    unsigned char byte;
    // create empty buffer byte
    while (file.read(reinterpret_cast<char*>(&byte), 1))
    { 
        // read from char ptr, 1 bit only 
        for (int i=7; i>=0; --i)
        {
            unsigned char mask = 1 << i;
            // create mask bit by left shifting 1 by i, to check i'th bit
            if (byte & mask) output += '1'; // check using bitwise AND with bit in consideration
            else output += '0'; 
        }
    }
    // remove padding bits
    if (padding > 0 && padding < 8 && bitnumber > padding)
    {
        output = output.substr(0, output.length() - padding);
    }
    file.close();
    return output;
}
