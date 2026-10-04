#ifndef HEAP_H
#define HEAP_H

typedef struct Heap {
  int* arr;
  int size;
  int capacity;
} Heap;

Heap* heap_create(int capacity);

void heap_insert(Heap* heap, int value);

int heap_extract_min(Heap* heap);

int heap_is_empty(Heap* heap);

void heap_destroy(Heap* heap);

#endif
