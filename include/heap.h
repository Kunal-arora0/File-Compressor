#ifndef HEAP_H
#define HEAP_H
#include "huffman.h"

typedef struct Heap {
  HuffmanNode **arr;
  int size;
  int capacity;
} Heap;

Heap* heap_create(int capacity);

void heap_insert(Heap* heap, HuffmanNode *node);

HuffmanNode *heap_extract_min(Heap* heap);

int heap_is_empty(Heap* heap);

void heap_destroy(Heap* heap);

#endif
