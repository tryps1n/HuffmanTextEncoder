#include "bits/stdc++.h"
#include "fstream"
#include <cstdint>
#include <sys/types.h>
#include <unordered_map>

using namespace std;

#pragma pack(push, 1)
struct file_header 
{
    unsigned char magic_number[4] = {
        'H', 'U', 'F', 'F'
    };
    uint32_t mapNumber;
    uint32_t dataBits;
    int padding;
    // number of entries in freq table
};
#pragma pack(pop)

void write_bin_to_file(fstream& file, string bits, unordered_map<char, int>& freq_table)
{
    file_header header;
    header.mapNumber = freq_table.size();
    header.dataBits = bits.length();
    header.padding = 8 - (bits.length() % 8);
    file.write(reinterpret_cast<char*>(&header), sizeof(header));
    //writes header class into file

    for (const auto& pair : freq_table){
        char char_val = pair.first;
        int freq = pair.second;
        file.write(reinterpret_cast<const char*>(&char_val), 1);
        file.write(reinterpret_cast<const char*>(&freq), 4);
    }
    //writes freq table data into the file.

    unsigned char current_byte = 0; int bit_position = 0; 
    // create empty byte using unsigned char, which holds one byte
    for (char c : bits)
    {
        current_byte = (current_byte << 1) | (c - '0'); 
        // shift current_byte by 1 to make room for new bit
        // bitwise OR -> add one byte to the newly created 0 at the right
        // (c-'0') gives binary for either 1 or 0, since c can be either '1' or '0'
        bit_position++; 

        if (bit_position == 8){
            file.write(reinterpret_cast<char*>(&current_byte), 1);
            // write the byte into the bin file by casting unsigned char pointer into a
            // char pointer pointing to the address, write 1 bit starting from address
            current_byte = 0; bit_position = 0;
        }
    }
    if (bit_position != 0)
    {
        current_byte <<= (8 - bit_position); //shifts to MSB
        file.write(reinterpret_cast<char*>(&current_byte), 1);
    }
}

string read_bin_from_file(fstream& file)
{
    // save current position
    std::streampos originalPos = file.tellg();
    
    // go to beginning to read everything
    file.seekg(0, std::ios::beg);

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
            if (byte & mask) // check using bitwise AND with bit in consideration
            {
                output += '1';
            }
            else 
            {
                output += '0';
            }
        }
    }
    file.seekg(originalPos);
    return output;
}

