#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"

#include "Drivers/Display.h"
#include "Common/BootInfo.h"
#include "Descriptors/GDT.h"

#include <stdbool.h>
#include <string.h>

extern uint64_t KernelStart;
extern uint64_t KernelEnd;
extern void kmain();

static struct
{
    PageTableManager PageTableManager;
} Kernel = {0};

static void InitMemory(BootInfo* bootInfo)
{
    uint64_t mMapEntries = bootInfo->memoryMapSize / bootInfo->descriptorSize;
    uint64_t kernelSize = (uint64_t)&KernelEnd - (uint64_t)&KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;

    PageFrameAllocator_ReadEFIMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->descriptorSize);
    PageFrameAllocator_LockPages(&KernelStart, kernelPages);

    PageTable* pageTableMap4 = (PageTable*)PageFrameAllocator_RequestPage();
    memset(pageTableMap4, 0, 0x1000);
    PageTableManager manager = { pageTableMap4 };

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->memoryMap, mMapEntries, bootInfo->descriptorSize); t+= 0x1000){
        PageTableManager_MapMemory(&manager, (void*)t, (void*)t);
    }

    uint64_t frameBufferBase = (uint64_t)bootInfo->frameBuffer->baseAddress;
    uint64_t frameBufferSize = (uint64_t)bootInfo->frameBuffer->bufferSize + 0x1000;
    PageFrameAllocator_LockPages((void*)frameBufferBase, frameBufferSize/ 0x1000 + 1);
    for (uint64_t i = frameBufferBase; i < frameBufferBase + frameBufferSize; i += 4096){
        PageTableManager_MapMemory(&manager, (void*)i, (void*)i);
    }

    asm ("mov %0, %%cr3" : : "r" (pageTableMap4));
    Kernel.PageTableManager = manager;
}

void _start(BootInfo* bootInfo)
{
    GDTDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(GDT)-1;
    gdtDescriptor.offset = (uint64_t)&GlobalDescriptorTable;
    LoadGDT(&gdtDescriptor);   

    InitMemory(bootInfo);
    FrameBuffer_Init(bootInfo->frameBuffer);
    TextRenderer_Init(bootInfo->font);

    TextRenderer_RenderText("Kernel Initialized Successfully.", 0, 0);
    while (true);
}