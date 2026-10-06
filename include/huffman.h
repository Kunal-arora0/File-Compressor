#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct HuffmanNode{
    unsigned char character;
    unsigned long frequency;


    struct HuffmanNode *left;
    struct HuffmanNode *right;
}HuffmanNode;

HuffmanNode *huffman_create_node(unsigned char character, unsigned long frequency);
HuffmanNode *build_huffman_tree(unsigned long freq[256]);
void generate_codes(HuffmanNode *node , char *code, int depth);
#endif
