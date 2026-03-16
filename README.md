# Huffman Text Encoder

Uses the Huffman Encoding algorithm to encode .txt files and outputs a .huff encrypted binary file. 

---

## Usage: 

1. cd to /src and compile main.cpp
2. run `./main <flag> <filename>`

### Flags:  
    1. -e   
        encode (accepts .txt files)   
    2. -d  
        decode (accepts .huff files, other files wont work as program checks for HUFF header)  