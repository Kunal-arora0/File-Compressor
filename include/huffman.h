#ifndef HUFFMAN_H
#define HUFFMAN_H

typedef struct HuffmanNode{
    unsigned char character;
    unsigned long frequency;


    struct HuffmanNode *left;
    struct HuffmanNode *right;
}HuffmanNode;

HuffmanNode *huffman_create_node(unsigned char character, unsigned long frequency);
#endif
