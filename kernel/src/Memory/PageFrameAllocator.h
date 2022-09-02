#pragma once

#include <stdint.h>

#include "EFIMemory.h"
#include "Bitmap.h"
#include "Memory.h"

typedef struct PageFrameAllocator_t
{
    Bitmap pageBitMap;
} PageFrameAllocator;

void PageFrameAllocator_ReadEFIMemoryMap(PageFrameAllocator* allocator, EFI_MEMORY_DESCRIPTOR* memoryMap, 
                                         size_t memoryMapSize, size_t descriptorSize);
void PageFrameAllocator_InitBitmap(PageFrameAllocator* allocator, size_t bitmapSize, void* bufferAddress);
void* PageFrameAllocator_RequestPage(PageFrameAllocator* allocator);

void PageFrameAllocator_FreePage(PageFrameAllocator* allocator, void* address);
void PageFrameAllocator_LockPage(PageFrameAllocator* allocator, void* address);
void PageFrameAllocator_FreePages(PageFrameAllocator* allocator, void* address, uint64_t pageCount);
void PageFrameAllocator_LockPages(PageFrameAllocator* allocator, void* address, uint64_t pageCount);

uint64_t PageFrameAllocator_GetFreeRAM();
uint64_t PageFrameAllocator_GetUsedRAM();
uint64_t PageFrameAllocator_GetReservedRAM();

void PageFrameAllocator_ReservePage(PageFrameAllocator* allocator, void* address);
void PageFrameAllocator_UnreservePage(PageFrameAllocator* allocator, void* address);
void PageFrameAllocator_ReservePages(PageFrameAllocator* allocator, void* address, uint64_t pageCount);
void PageFrameAllocator_UnreservePages(PageFrameAllocator* allocator, void* address, uint64_t pageCount);