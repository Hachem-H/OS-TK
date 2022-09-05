#include "PageFrameAllocator.h"

static struct
{
    uint64_t freeMemory;
    uint64_t reservedMemory;
    uint64_t usedMemory;
    uint64_t pageBitmapIndex;
    unsigned Initialized;
} Info = {0};

static PageFrameAllocator GlobalAllocator = {0};

void PageFrameAllocator_ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, size_t memoryMapSize, size_t descriptorSize)
{
    if (Info.Initialized) 
        return;
    Info.Initialized = true;

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
    Info.freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / 4096 / 8 + 1;

    PageFrameAllocator_InitBitmap(bitmapSize, largestFreeMemSeg);
    PageFrameAllocator_ReservePages(0, memorySize / 4096 + 1);

    for (uint64_t i = 0; i < memoryMapEntries; ++i)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)memoryMap + (i * descriptorSize));
        if (descriptor->type == 7)
            PageFrameAllocator_UnreservePages(descriptor->physicalAddress, descriptor->pagesAmount);
    }
    
    PageFrameAllocator_ReservePages(0, 0x100); 
    PageFrameAllocator_LockPages(GlobalAllocator.pageBitMap.buffer, GlobalAllocator.pageBitMap.size / 4096 + 1);
}

void PageFrameAllocator_InitBitmap(size_t bitmapSize, void* bufferAddress)
{
    GlobalAllocator.pageBitMap.size = bitmapSize;
    GlobalAllocator.pageBitMap.buffer = (uint8_t*)bufferAddress;
    for (size_t i = 0; i < bitmapSize; ++i)
        *(uint8_t*)(GlobalAllocator.pageBitMap.buffer + i) = 0;
}

void* PageFrameAllocator_RequestPage()
{
    for (; Info.pageBitmapIndex < GlobalAllocator.pageBitMap.size * 8; Info.pageBitmapIndex++)
    {
        if (Bitmap_Get(&GlobalAllocator.pageBitMap, Info.pageBitmapIndex) == true)
            continue;

        PageFrameAllocator_LockPage((void*)(Info.pageBitmapIndex * 4096));
        return (void*)(Info.pageBitmapIndex * 4096);
    }

    return NULL;
}

void PageFrameAllocator_FreePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, index) == false) 
        return;

    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, false))
    {
        Info.freeMemory += 4096;
        Info.usedMemory -= 4096;
        if (Info.pageBitmapIndex > index) 
            Info.pageBitmapIndex = index;
    }
}

void PageFrameAllocator_FreePages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; i++)
        PageFrameAllocator_FreePage((void*)((uint64_t)address + (i * 4096)));
}

void PageFrameAllocator_LockPage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, index) == true) 
        return;

    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, true))  
    {
        Info.freeMemory -= 4096;
        Info.usedMemory += 4096;
    }
}

void PageFrameAllocator_LockPages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; i++)
        PageFrameAllocator_LockPage((void*)((uint64_t)address + (i * 4096)));
}

void PageFrameAllocator_UnreservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, index) == false) 
        return;

    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, false)) 
    {
        Info.freeMemory += 4096;
        Info.reservedMemory -= 4096;
        if (Info.pageBitmapIndex > index) 
            Info.pageBitmapIndex = index;
    }
}

void PageFrameAllocator_UnreservePages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; i++)
        PageFrameAllocator_UnreservePage((void*)((uint64_t)address + (i * 4096)));
}

void PageFrameAllocator_ReservePage(void* address)
{
    uint64_t index = (uint64_t)address / 4096;
    if (Bitmap_Get(&GlobalAllocator.pageBitMap, index) == true) 
        return;

    if (Bitmap_Set(&GlobalAllocator.pageBitMap, index, true)) 
    {
        Info.freeMemory -= 4096;
        Info.reservedMemory += 4096;
    }
}

void PageFrameAllocator_ReservePages(void* address, uint64_t pageCount)
{
    for (uint64_t i = 0; i < pageCount; i++)
        PageFrameAllocator_ReservePage((void*)((uint64_t)address + (i * 4096)));
}

uint64_t PageFrameAllocator_GetFreeRAM()     { return Info.freeMemory;     }
uint64_t PageFrameAllocator_GetUsedRAM()     { return Info.usedMemory;     }
uint64_t PageFrameAllocator_GetReservedRAM() { return Info.reservedMemory; }