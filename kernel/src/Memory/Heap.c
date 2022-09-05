#include "Heap.h"

#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"

static struct
{
    void* start;
    void* end;
    HeapSegmentHeader* lastHeader;
} Heap = {0};

void HeapSegmentHeader_CombineForward(HeapSegmentHeader* header)
{
    if (header->next == NULL) 
        return;
    if (!header->next->free) 
        return;

    if (header->next == Heap.lastHeader) 
        Heap.lastHeader = header;
    if (header->next->next != NULL)
        header->next->next->last = header;

    header->size = header->size + header->next->size + sizeof(HeapSegmentHeader);
}

void HeapSegmentHeader_CombineBackward(HeapSegmentHeader* header)
{
    if (header->last != NULL && header->last->free) 
        HeapSegmentHeader_CombineForward(header->last);
}

void* HeapAllocate(size_t size)
{
    if (size % 0x10 > 0)
    {
        size -= (size % 0x10);
        size += 0x10;
    }

    if (size == 0) 
        return NULL;

    HeapSegmentHeader* currentSeg = (HeapSegmentHeader*) Heap.start;
    while(true)
    {
        if(currentSeg->free)
        {
            if (currentSeg->size > size)
            {
                HeapSegmentHeader_Split(currentSeg, size);
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegmentHeader));
            }

            if (currentSeg->size == size)
            {
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegmentHeader));
            }
        }

        if (currentSeg->next == NULL) 
            break;
        currentSeg = currentSeg->next;
    }

    ExpandHeap(size);
    return HeapAllocate(size);
}

void HeapFree(void* address)
{
    HeapSegmentHeader* segment = (HeapSegmentHeader*)address - 1;
    segment->free = true;
    HeapSegmentHeader_CombineForward(segment);
    HeapSegmentHeader_CombineBackward(segment);
}

HeapSegmentHeader* HeapSegmentHeader_Split(HeapSegmentHeader* header, size_t splitLength)
{
    if (splitLength < 0x10) 
        return NULL;

    int64_t splitSegLength = header->size - splitLength - (sizeof(HeapSegmentHeader));
    if (splitSegLength < 0x10) return NULL;

    HeapSegmentHeader* newSplit = (HeapSegmentHeader*)((size_t)header+splitLength+sizeof(HeapSegmentHeader));
    header->next->last = newSplit;
    newSplit->next = header->next;
    header->next = newSplit;
    newSplit->last = header;
    newSplit->size = splitSegLength;
    newSplit->free = header->free;
    header->size = splitLength;

    if (Heap.lastHeader == header) 
        header = newSplit;
    return newSplit;
}

void InitHeap(void* heapAddress, size_t pagesAmount)
{
    void* position = heapAddress;
    for (size_t i = 0; i < pagesAmount; i++)
    {
        PageTableManager_MapMemory(position, PageFrameAllocator_RequestPage());
        position = (void*)((size_t)position + 0x1000);
    }

    size_t heapLength = pagesAmount * 0x1000;
    Heap.start = heapAddress;
    Heap.end = (void*)((size_t)Heap.start + heapLength);

    HeapSegmentHeader* startSeg = (HeapSegmentHeader*)heapAddress;
    startSeg->size  = heapLength - sizeof(HeapSegmentHeader);
    startSeg->next  = NULL;
    startSeg->last  = NULL;
    startSeg->free  = true;
    Heap.lastHeader = startSeg;
}

void ExpandHeap(size_t size)
{
    if (size % 0x1000) 
    {
        size -= size % 0x1000;
        size += 0x1000;
    }

    size_t pageCount = size / 0x1000;
    HeapSegmentHeader* newSegment = (HeapSegmentHeader*)Heap.end;

    for (size_t i = 0; i < pageCount; i++)
    {
        PageTableManager_MapMemory(Heap.end, PageFrameAllocator_RequestPage());
        Heap.end = (void*)((size_t)Heap.end + 0x1000);
    }

    newSegment->free      = true;
    newSegment->last      = Heap.lastHeader;
    Heap.lastHeader->next = newSegment;
    Heap.lastHeader       = newSegment;
    newSegment->next      = NULL;
    newSegment->size      = size - sizeof(HeapSegmentHeader);
    HeapSegmentHeader_CombineBackward(newSegment);
}
