#include <heap.h>
#include <stdio.h>
#include <stdlib.h>

Heap* heap_create(int capacity) {
    Heap* heap = malloc(sizeof(Heap));

    if (heap == NULL) {
        return NULL;
    }

    heap->arr = malloc(capacity * sizeof(HuffmanNode *));

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

    if (heap->arr[parentidx]->frequency > heap->arr[index]->frequency) {
        HuffmanNode *temp  = heap->arr[parentidx];
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

    if (lchild < heap->size && heap->arr[lchild]->frequency < heap->arr[smallest]->frequency) {
        smallest = lchild;
    }

    if (rchild < heap->size && heap->arr[rchild]->frequency < heap->arr[smallest]->frequency) {
        smallest = rchild;
    }
    if (smallest != index) {
        HuffmanNode *temp = heap->arr[index];
        heap->arr[index] = heap->arr[smallest];
        heap->arr[smallest] = temp;

        heapify_down(heap, smallest);
    }
}

void heap_insert(Heap* heap, HuffmanNode *node) {
    if (heap->size >= heap->capacity) {
        printf("Heap is full");
        return;
    }
    heap->arr[heap->size] = node;
    heap->size++;

    heapify_up(heap, heap->size - 1);
}

HuffmanNode *heap_extract_min(Heap* heap) {
    if (heap->size == 0) {
        return NULL;
    }
    HuffmanNode *element = heap->arr[0];
    heap->arr[0] = heap->arr[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return element;
}

int heap_is_empty(Heap *heap){
    return heap->size ==0;
}


void heap_destroy(Heap *heap)
{
    if (heap == NULL) {
        return;
    }

    free(heap->arr);
    free(heap);
}
