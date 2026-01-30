#include "bits/stdc++.h"
#include "fstream"
#include <fstream>

using namespace std;

void write_bin_to_file(fstream& file, string bits)
{
    unsigned char current_byte; int bit_position = 0;
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
