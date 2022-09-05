#pragma once

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef struct HeapSegmentHeader_t
{
    size_t size;
    bool free;

    struct HeapSegmentHeader_t* next;
    struct HeapSegmentHeader_t* last;
} HeapSegmentHeader;

void HeapSegmentHeader_CombineForward(HeapSegmentHeader* header);
void HeapSegmentHeader_CombineBackward(HeapSegmentHeader* header);
HeapSegmentHeader* HeapSegmentHeader_Split(HeapSegmentHeader* header, size_t splitLength);

void InitHeap(void* heapAddress, size_t pagesAmount);
void ExpandHeap(size_t size);

void* HeapAllocate(size_t size);
void HeapFree(void* address);