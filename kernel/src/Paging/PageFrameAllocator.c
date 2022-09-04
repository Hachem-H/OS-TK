#include "PageFrameAllocator.h"

static PageFrameAllocator GlobalAllocator;
static struct 
{
    uint64_t FreeMemory;
    uint64_t UsedMemory;
    uint64_t ReserverdMemory;

    uint64_t pageBitmapIndex;    
    unsigned IsInitialized;
} info = {0};

void SetGlobalPageFrameAllocator(PageFrameAllocator* allocator)
{
    GlobalAllocator.pageBitMap = allocator->pageBitMap;
}

void PageFrameAllocator_ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, size_t memoryMapSize, size_t descriptorSize)
{
    if (info.IsInitialized)
        return;
    info.IsInitialized = true;
    uint64_t memoryMapEntries = memoryMapSize / descriptorSize;

    void*  largestFreeMemSeg     = NULL;
    size_t largestFreeMemSegSize = 0;

    for (int i = 0; i < memoryMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap+i*descriptorSize);
        if (descriptor->type == 7)
            if (descriptor->pagesAmount * 4096 > largestFreeMemSegSize)
            {
                largestFreeMemSeg = descriptor->physicalAddress;
                largestFreeMemSegSize = descriptor->pagesAmount * 4096;
            }
    }

    uint64_t memorySize = GetMemorySize(memoryMap, memoryMapEntries, descriptorSize);
    info.FreeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    PageFrameAllocator_InitBitmap(bitmapSize, largestFreeMemSeg);
    PageFrameAllocator_LockPages(&GlobalAllocator.pageBitMap, GlobalAllocator.pageBitMap.size / 4096 + 1);

    for (int i = 0; i < memoryMapEntries; i++)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap+i*descriptorSize);
        if (descriptor->type != 7)
            PageFrameAllocator_ReservePages(descriptor->physicalAddress, descriptor->pagesAmount);
    } 
}

void PageFrameAllocator_InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    GlobalAllocator.pageBitMap.size = bitmapSize;
    GlobalAllocator.pageBitMap.buffer = (uint8_t*)bufferAddress;

    for (int i = 0; i < bitmapSize; i++)
        GlobalAllocator.pageBitMap.buffer[i] = 0;
}

void* PageFrameAllocator_RequestPage()
{
    for (; info.pageBitmapIndex < GlobalAllocator.pageBitMap.size*8; info.pageBitmapIndex++)
    {
        if (Bitmap_Get(&GlobalAllocator.pageBitMap, info.pageBitmapIndex) == true)
            continue;
        PageFrameAllocator_LockPage((void*)(info.pageBitmapIndex* 4096));
        return (void*)(info.pageBitmapIndex*4096);
    }

    return NULL;
}

void PageFrameAllocator_FreePage(void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, i) == false)
        return;
    
    if (Bitmap_Set(&GlobalAllocator.pageBitMap, i, false))
    {
        info.FreeMemory += 4096;
        info.UsedMemory -= 4096;
        if (info.pageBitmapIndex > i)
            info.pageBitmapIndex = i;
    }
}

void PageFrameAllocator_LockPage(void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, i) == true)
        return;
    
    if (Bitmap_Set(&GlobalAllocator.pageBitMap, i, true))
    {
        info.FreeMemory -= 4096;
        info.UsedMemory += 4096;
    }
}

void PageFrameAllocator_FreePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
        PageFrameAllocator_FreePage((void*)((uint64_t)address + (i*4096)));
}

void PageFrameAllocator_LockPages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
        PageFrameAllocator_LockPage((void*)((uint64_t)address + (i*4096)));
}

uint64_t PageFrameAllocator_GetFreeRAM()     { return info.FreeMemory;      }
uint64_t PageFrameAllocator_GetUsedRAM()     { return info.UsedMemory;      }
uint64_t PageFrameAllocator_GetReservedRAM() { return info.ReserverdMemory; }

void PageFrameAllocator_ReservePage(void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, i) == true)
        return;
    
    if (Bitmap_Set(&GlobalAllocator.pageBitMap, i, true))
    {
        info.FreeMemory -= 4096;
        info.ReserverdMemory += 4096;
    }
}

void PageFrameAllocator_UnreservePage(void* address)
{
    uint64_t i = (uint64_t)address/4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, i) == false)
        return;
    
    if (Bitmap_Set(&GlobalAllocator.pageBitMap, i, false))
    {
        info.FreeMemory += 4096;
        info.ReserverdMemory -= 4096;
        if (info.pageBitmapIndex > i)
            info.pageBitmapIndex = i;
    }
}

void PageFrameAllocator_ReservePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
        PageFrameAllocator_ReservePage((void*)((uint64_t)address + (i*4096)));
}

void PageFrameAllocator_UnreservePages(void* address, uint64_t pageCount)
{
    for (int i = 0; i < pageCount; i++)
        PageFrameAllocator_UnreservePage((void*)((uint64_t)address + (i*4096)));
}
