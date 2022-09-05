#include "PageFrameAllocator.h"

uint64_t freeMemory      = 0;
uint64_t reservedMemory  = 0;
uint64_t usedMemory      = 0;
uint64_t pageBitmapIndex = 0;
unsigned Initialized     = 0;

PageFrameAllocator GlobalAllocator = {0};

void PageFrameAllocator_ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, size_t memoryMapSize, size_t descriptorSize){
    if (Initialized) 
        return;
    Initialized = true;

    uint64_t memoryMapEntries = memoryMapSize / descriptorSize;

    void* largestFreeMemSeg = NULL;
    uint64_t largestFreeMemSegSize = 0;

    for (uint64_t i = 0; i < memoryMapEntries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap + (i * descriptorSize));
        if (descriptor->type == 7)
            if (descriptor->pagesAmount * 4096 > largestFreeMemSegSize)
            {
                largestFreeMemSeg = descriptor->physicalAddress;
                largestFreeMemSegSize = descriptor->pagesAmount * 4096;
            }
    }

    uint64_t memorySize = GetMemorySize(memoryMap, memoryMapEntries, descriptorSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    PageFrameAllocator_InitBitmap(bitmapSize, largestFreeMemSeg);
    PageFrameAllocator_LockPages(&GlobalAllocator.pageBitMap, GlobalAllocator.pageBitMap.size / 4096 + 1);

    for (uint64_t i = 0; i < memoryMapEntries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap + (i * descriptorSize));
        if (descriptor->type != 7)
            PageFrameAllocator_ReservePages(descriptor->physicalAddress, descriptor->pagesAmount);
    }
}

void PageFrameAllocator_InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    GlobalAllocator.pageBitMap.size = bitmapSize;
    GlobalAllocator.pageBitMap.buffer = (uint8_t*)bufferAddress;

    for (int i = 0; i < bitmapSize; ++i)
        *(uint8_t*)(GlobalAllocator.pageBitMap.buffer + i) = 0;
}

void* PageFrameAllocator_RequestPage()
{
    for (; pageBitmapIndex < GlobalAllocator.pageBitMap.size * 8; pageBitmapIndex++)
    {
        if (Bitmap_Get(&GlobalAllocator.pageBitMap, pageBitmapIndex) == true)
            continue;
        PageFrameAllocator_LockPage((void*)(pageBitmapIndex * 4096));
        return (void*)(pageBitmapIndex * 4096);
    }

    return NULL;
}

void PageFrameAllocator_FreePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, pageBitmapIndex) == false) 
        return;

    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, false))
    {
        freeMemory += 4096;
        usedMemory -= 4096;

        if (pageBitmapIndex > index) 
            pageBitmapIndex = index;
    }
}

void PageFrameAllocator_FreePages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; ++i)
        PageFrameAllocator_FreePage((void*)((uint64_t)address + (i * 4096)));
}

void PageFrameAllocator_LockPage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, pageBitmapIndex) == true) 
        return;
    
    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, true))
    {
        freeMemory -= 4096;
        usedMemory += 4096;
    }
}

void PageFrameAllocator_LockPages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; ++i)
        PageFrameAllocator_LockPage((void*)((uint64_t)address + (i * 4096)));
}

void PageFrameAllocator_UnreservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, pageBitmapIndex) == false) 
        return;
        
    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, false))
    {
        freeMemory += 4096;
        reservedMemory -= 4096;

        if (pageBitmapIndex > index) 
            pageBitmapIndex = index;
    }
}

void PageFrameAllocator_UnreservePages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; ++i)
        PageFrameAllocator_UnreservePage((void*)((uint64_t)address + (i * 4096)));
}

void PageFrameAllocator_ReservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, pageBitmapIndex) == true) 
        return;

    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, true))
    {
        freeMemory -= 4096;
        reservedMemory += 4096;
    }
}

void PageFrameAllocator_ReservePages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; ++i)
        PageFrameAllocator_ReservePage((void*)((uint64_t)address + (i * 4096)));
}

uint64_t PageFrameAllocator_GetFreeRAM()     { return freeMemory;     }
uint64_t PageFrameAllocator_GetUsedRAM()     { return usedMemory;     }
uint64_t PageFrameAllocator_GetReservedRAM() { return reservedMemory; }
