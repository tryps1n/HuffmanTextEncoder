# Huffman Text Encoder

Uses the Huffman Encoding algorithm to encode `.txt` files to `.huff` binary encoded files and vice versa.  

---

## Usage: 

1. cd to /src and compile main.cpp
2. run `./main <flag> <filename>`

### Flags:  
    1. -e   
        encode (accepts .txt files)   
    2. -d  
        decode (accepts .huff files, other files wont work as program checks for HUFF header) 

--- 

## Mechanism

Counts characters present in the file and generates a Huffman tree using all of the frequencies, assigning binary codes to each character with length depending on the character's frequency.  

Rewrites the file using each character's assigned code and generates a `.huff.` file.  

The `.huff` file contains a header with the following data:  
1. Custom magic number `HUFF` which the program identifies
2. Information about the total number of bits, total number of frequency table entries, and the number of padding bits used at the end of the file
3. All of the frequency table pairs in the format `<character><frequency>`

In other words, the program stores the generated frequency table entries into the header of each encoded `.huff` file. 

During decoding, the tree is regenerated using the reconstructed frequency table obtained from the header, and is used to decode the encoded `.huff` file to form a `.txt` file. 