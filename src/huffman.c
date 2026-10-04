#include<stdio.h>
#include<stdlib.h>
#include "huffman.h"


HuffmanNode *huffman_create_node(unsigned char character, unsigned long frequency){
    HuffmanNode *huff =  malloc(sizeof(HuffmanNode));

    if(huff == NULL) return NULL;

    huff->character = character;
    huff->frequency = frequency;

    huff->left = NULL;
    huff->right = NULL;

    return huff;
}
