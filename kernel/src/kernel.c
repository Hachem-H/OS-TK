#include "Drivers/Display.h"

#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"

#include "Common/BootInfo.h"

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

    PageTable* PML4 = (PageTable*)PageFrameAllocator_RequestPage();
    memset(PML4, 0, 0x1000);
    PageTableManager manager = { PML4 };

    for (uint64_t t = 0; t < GetMemorySize(bootInfo->memoryMap, mMapEntries, bootInfo->descriptorSize); t+= 0x1000){
        PageTableManager_MapMemory(&manager, (void*)t, (void*)t);
    }

    uint64_t fbBase = (uint64_t)bootInfo->frameBuffer->baseAddress;
    uint64_t fbSize = (uint64_t)bootInfo->frameBuffer->bufferSize + 0x1000;
    PageFrameAllocator_LockPages((void*)fbBase, fbSize/ 0x1000 + 1);
    for (uint64_t t = fbBase; t < fbBase + fbSize; t += 4096){
        PageTableManager_MapMemory(&manager, (void*)t, (void*)t);
    }

    asm ("mov %0, %%cr3" : : "r" (PML4));
    Kernel.PageTableManager = manager;
}

void _start(BootInfo* bootInfo)
{
    InitMemory(bootInfo);
    TextRenderer_InitWith(bootInfo->frameBuffer, bootInfo->font);

    TextRenderer_RenderText("Kernel Initialized Successfully.", 0, 0);
    while (true);
}
