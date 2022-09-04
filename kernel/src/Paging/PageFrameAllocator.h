#pragma once

#include <stdint.h>

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"

typedef struct PageFrameAllocator_t
{
    Bitmap pageBitMap;
} PageFrameAllocator;

void SetGlobalPageFrameAllocator(PageFrameAllocator* allocator);

void PageFrameAllocator_ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* memoryMap, 
                                         size_t memoryMapSize, size_t descriptorSize);
void PageFrameAllocator_InitBitmap(size_t bitmapSize, void* bufferAddress);
void* PageFrameAllocator_RequestPage();

void PageFrameAllocator_FreePage(void* address);
void PageFrameAllocator_LockPage(void* address);
void PageFrameAllocator_FreePages(void* address, uint64_t pageCount);
void PageFrameAllocator_LockPages(void* address, uint64_t pageCount);

uint64_t PageFrameAllocator_GetFreeRAM();
uint64_t PageFrameAllocator_GetUsedRAM();
uint64_t PageFrameAllocator_GetReservedRAM();

void PageFrameAllocator_ReservePage(void* address);
void PageFrameAllocator_UnreservePage(void* address);
void PageFrameAllocator_ReservePages(void* address, uint64_t pageCount);
void PageFrameAllocator_UnreservePages(void* address, uint64_t pageCount);