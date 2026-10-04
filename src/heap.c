#include <heap.h>
#include <stdio.h>
#include <stdlib.h>

Heap* heap_create(int capacity) {
  Heap* heap = malloc(sizeof(Heap));

  if (heap == NULL) {
    return NULL;
  }

  heap->arr = malloc(capacity * sizeof(int));

  if (heap->arr == NULL) {
    free(heap);
    return NULL;
  }

  heap->size = 0;
  heap->capacity = capacity;

  return heap;
}

static void heapify_up(Heap* heap, int index) {
  if (index == 0) return;

  int parentidx = (index - 1) / 2;

  if (heap->arr[parentidx] > heap->arr[index]) {
    int temp = heap->arr[parentidx];
    heap->arr[parentidx] = heap->arr[index];
    heap->arr[index] = temp;

    heapify_up(heap, parentidx);
  }
}

static void heapify_down(Heap* heap, int index) {
  if (index >= heap->size) {
    return;
  }

  int lchild = 2 * index + 1;
  int rchild = 2 * index + 2;

  int smallest = index;

  if (lchild < heap->size && heap->arr[lchild] < heap->arr[smallest]) {
    smallest = lchild;
  }

  if (rchild < heap->size && heap->arr[rchild] < heap->arr[smallest]) {
    smallest = rchild;
  }
  Z if (smallest != index) {
    int temp = heap->arr[index];
    heap->arr[index] = heap->arr[smallest];
    heap->arr[smallest] = temp;

    heapify_down(heap, smallest);
  }
}

void heap_insert(Heap* heap, int value) {
  if (heap->size >= heap->capacity) {
    printf("Heap is full");
    return;
  }
  heap->arr[heap->size] = value;
  heap->size++;

  heapify_up(heap, heap->size - 1);
}

int heap_extract_min(Heap* heap) {
  if (heap->size == 0) {
    return -1;
  }
  int element = heap->arr[0];
  heap->arr[0] = heap->arr[heap->size - 1];
  heap->size--;
  heapify_down(heap, 0);
  return element;
}
