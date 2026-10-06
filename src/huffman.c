#include<stdio.h>
#include<stdlib.h>
#include "huffman.h"
#include "heap.h"


HuffmanNode *huffman_create_node(unsigned char character, unsigned long frequency){
    HuffmanNode *huff =  malloc(sizeof(HuffmanNode));

    if(huff == NULL) return NULL;

    huff->character = character;
    huff->frequency = frequency;

    huff->left = NULL;
    huff->right = NULL;

    return huff;
}

HuffmanNode *build_huffman_tree(unsigned long freq[256]){
    Heap *heap = heap_create(256);

    for(int i = 0; i<256 ;i++){
        if(freq[i]  > 0){
            heap_insert(heap, huffman_create_node((unsigned char)i , freq[i]));
        }
    }

    if (heap->size == 0) {
        heap_destroy(heap);
        return NULL;
    }


    while(heap->size > 1){
        HuffmanNode *smallest  = heap_extract_min(heap);
        HuffmanNode *secondsmallest  = heap_extract_min(heap);


        HuffmanNode *combined = huffman_create_node('\0' , smallest->frequency + secondsmallest->frequency);
        combined->left = smallest;
        combined->right = secondsmallest;

        heap_insert(heap, combined);
    }

    HuffmanNode *root = heap_extract_min(heap);
    heap_destroy(heap);
    return root;
}

void generate_codes(HuffmanNode *node , char *code , int depth){
    if(node == NULL){
        return;
    }

    if(node->left == NULL && node->right == NULL){
        code[depth] = '\0';
        printf("%c -> %s\n" , node->character, code);
        return;
    }

    code[depth] = '0';
    generate_codes(node->left, code, depth+1);

    code[depth] = '1';
    generate_codes(node->right, code, depth+1);
}
