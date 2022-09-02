#include "PageFrameAllocator.h"

static struct 
{
    uint64_t FreeMemory;
    uint64_t UsedMemory;
    uint64_t ReserverdMemory;
    unsigned IsInitialized;
} info = {0};

void PageFrameAllocator_ReadEFIMemoryMap(PageFrameAllocator* allocator, EFI_MEMORY_DESCRIPTOR* memoryMap, 
                                         size_t memoryMapSize, size_t descriptorSize)
{
    if (info.IsInitialized)
        return;
    info.IsInitialized = true;
    uint64_t memoryMapEntries = memoryMapSize / descriptorSize;

    void*  largestFreeMemSeg     = NULL;
    size_t largestFreeMemSegSize = 0;

    for (int i = 0; i < memoryMapEntries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap+i*descriptorSize);
        if (descriptor->type == 7)
            if (descriptor->pagesAmount * 4096 > largestFreeMemSegSize)
                largestFreeMemSeg = descriptor->physicalAddress;
                largestFreeMemSegSize = descriptor->pagesAmount * 4096;
    }

    uint64_t memorySize = GetMemorySize(memoryMap, memoryMapEntries, descriptorSize);
    info.FreeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    PageFrameAllocator_InitBitmap(allocator, bitmapSize, largestFreeMemSeg);
    PageFrameAllocator_LockPages(allocator, &allocator->pageBitMap, allocator->pageBitMap.size / 4096 + 1);

    for (int i = 0; i < memoryMapEntries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap+i*descriptorSize);
        if (descriptor->type != 7)
            PageFrameAllocator_ReservePages(allocator, descriptor->physicalAddress, descriptor->pagesAmount);
    } 
}

void PageFrameAllocator_InitBitmap(PageFrameAllocator* allocator, size_t bitmapSize, void* bufferAddress)
{
    allocator->pageBitMap.size = bitmapSize;
    allocator->pageBitMap.buffer = (uint8_t*)bufferAddress;

    for (int i = 0; i < bitmapSize; ++i)
        allocator->pageBitMap.buffer[i] = 0;
}

void* PageFrameAllocator_RequestPage(PageFrameAllocator* allocator)
{
    for (uint64_t i = 0; i < allocator->pageBitMap.size*8; ++i)
    {
        if (Bitmap_Get(&allocator->pageBitMap, i) == true)
            continue;
        PageFrameAllocator_LockPage(allocator, (void*)(i* 4096));
        return (void*)(i*4096);
    }

    return NULL;
}

void PageFrameAllocator_FreePage(PageFrameAllocator* allocator, void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&allocator->pageBitMap, i) == false)
        return;
    
    Bitmap_Set(&allocator->pageBitMap, i, false);
    info.FreeMemory += 4096;
    info.UsedMemory -= 4096;
}

void PageFrameAllocator_LockPage(PageFrameAllocator* allocator, void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&allocator->pageBitMap, i) == true)
        return;
    
    Bitmap_Set(&allocator->pageBitMap, i, true);
    info.FreeMemory -= 4096;
    info.UsedMemory += 4096;
}

void PageFrameAllocator_FreePages(PageFrameAllocator* allocator, void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
        PageFrameAllocator_FreePage(allocator, (void*)((uint64_t)address + (i*4096)));
}

void PageFrameAllocator_LockPages(PageFrameAllocator* allocator, void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
        PageFrameAllocator_LockPage(allocator, (void*)((uint64_t)address + (i*4096)));
}

uint64_t PageFrameAllocator_GetFreeRAM()     { return info.FreeMemory;      }
uint64_t PageFrameAllocator_GetUsedRAM()     { return info.UsedMemory;      }
uint64_t PageFrameAllocator_GetReservedRAM() { return info.ReserverdMemory; }

void PageFrameAllocator_ReservePage(PageFrameAllocator* allocator, void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&allocator->pageBitMap, i) == true)
        return;
    
    Bitmap_Set(&allocator->pageBitMap, i, true);
    info.FreeMemory -= 4096;
    info.ReserverdMemory += 4096;
}

void PageFrameAllocator_UnreservePage(PageFrameAllocator* allocator, void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&allocator->pageBitMap, i) == false)
        return;
    
    Bitmap_Set(&allocator->pageBitMap, i, false);
    info.FreeMemory += 4096;
    info.ReserverdMemory -= 4096;
}

void PageFrameAllocator_ReservePages(PageFrameAllocator* allocator, void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
        PageFrameAllocator_ReservePage(allocator, (void*)((uint64_t)address + (i*4096)));
}

void PageFrameAllocator_UnreservePages(PageFrameAllocator* allocator, void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; ++i)
        PageFrameAllocator_UnreservePage(allocator, (void*)((uint64_t)address + (i*4096)));
}
