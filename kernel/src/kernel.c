#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"

#include "Descriptors/GDT.h"
#include "Descriptors/IDT.h"

#include "Drivers/Display.h"
#include "Drivers/PIC.h"

#include "Core/Interrupts.h"
#include "Core/IO.h"

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

extern uint64_t KernelStart;
extern uint64_t KernelEnd;
extern void kmain();

static struct
{
    PageTableManager* PageTableManager;
    IDTRegister idtRegister;
} Kernel = {0};

#include "Common/BootInfo.h"

static void InitMemory(BootInfo* bootInfo)
{
    uint64_t memoryMapEntries = bootInfo->memoryMapSize / bootInfo->descriptorSize;
    PageFrameAllocator_ReadEFIMemoryMap(bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->descriptorSize);

    uint64_t kernelSize = (uint64_t)&KernelEnd - (uint64_t)&KernelStart;
    uint64_t kernelPages = (uint64_t)kernelSize / 4096 + 1;
    PageFrameAllocator_LockPages(&KernelStart, kernelPages);

    PageTable* pageTableMap4 = (PageTable*)PageFrameAllocator_RequestPage();
    memset(pageTableMap4, 0, 0x1000);

    static PageTableManager pageTableManager;
    pageTableManager.pageTableMap4 = pageTableMap4;

    for (uint64_t i = 0; i < GetMemorySize(bootInfo->memoryMap, memoryMapEntries, bootInfo->descriptorSize); i += 0x1000)
        PageTableManager_MapMemory(&pageTableManager, (void*)i, (void*)i);

    uint64_t frameBufferBase = (uint64_t)bootInfo->frameBuffer->baseAddress;
    uint64_t frameBufferSize = (uint64_t)bootInfo->frameBuffer->bufferSize + 0x1000;
    PageFrameAllocator_LockPages((void*)frameBufferBase, frameBufferSize/0x1000 + 1);

    for (uint64_t i = frameBufferBase; i < frameBufferBase + frameBufferSize; i += 4096)
        PageTableManager_MapMemory(&pageTableManager, (void*)i, (void*)i);

    asm ("mov %0, %%cr3" : : "r" (pageTableMap4));
    Kernel.PageTableManager = &pageTableManager;
}

static void SetIDTGate(void* handler, uint8_t entryOffset, uint8_t typeAttribute)
{
    IDTEntry* interrupt = (IDTEntry*)(Kernel.idtRegister.offset + entryOffset * sizeof(IDTEntry));
    IDTEntry_SetOffset(interrupt, (uint64_t) handler);
    interrupt->typeAttribute = typeAttribute;
    interrupt->selector =  0x08;
}

static void ISRInstall()
{
    Kernel.idtRegister.limit = 0x0FFF;
    Kernel.idtRegister.offset = (uint64_t)PageFrameAllocator_RequestPage();

    SetIDTGate((void*)PageFault_Handler, 0xE, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)DoubleFault_Handler, 0x8, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)GeneralProtectionFault_Handler, 0xE, IDT_TypeAttribute_InterruptGate);

    asm ("lidt %0" : : "m" (Kernel.idtRegister));
}

void _start(BootInfo* bootInfo)
{
    FrameBuffer_Init(bootInfo->frameBuffer);
    TextRenderer_Init(bootInfo->font);

    GDTDescriptor gdtDescriptor;
    gdtDescriptor.size = sizeof(GDT)-1;
    gdtDescriptor.offset = (uint64_t)&GlobalDescriptorTable;
    LoadGDT(&gdtDescriptor);   

    InitMemory(bootInfo);
    ISRInstall();
  
    asm("int $0x0E");

    for (;;);
}
