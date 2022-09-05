#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"
#include "Memory/Heap.h"

#include "Descriptors/GDT.h"
#include "Descriptors/IDT.h"

#include "Drivers/Display.h"
#include "Drivers/PIC.h"

#include "Core/Interrupts.h"
#include "Core/IO.h"

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern uint64_t KernelStart;
extern uint64_t KernelEnd;
extern void kmain();

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
    GlobalPageTableManager = pageTableManager;

    for (uint64_t i = 0; i < GetMemorySize(bootInfo->memoryMap, memoryMapEntries, bootInfo->descriptorSize); i += 0x1000)
        PageTableManager_MapMemory((void*)i, (void*)i);

    uint64_t frameBufferBase = (uint64_t)bootInfo->frameBuffer->baseAddress;
    uint64_t frameBufferSize = (uint64_t)bootInfo->frameBuffer->bufferSize + 0x1000;
    PageFrameAllocator_LockPages((void*)frameBufferBase, frameBufferSize/0x1000 + 1);

    for (uint64_t i = frameBufferBase; i < frameBufferBase + frameBufferSize; i += 4096)
        PageTableManager_MapMemory((void*)i, (void*)i);

    asm ("mov %0, %%cr3" : : "r" (pageTableMap4));
}

static void ISRInstall()
{
    static IDTRegister idtRegister;
    idtRegister.limit = 0x0FFF;
    idtRegister.offset = (uint64_t)PageFrameAllocator_RequestPage();

#define SetIDTGate(handler, entryOffset, attribute)                                         \
    {                                                                                       \
        IDTEntry* interrupt = (IDTEntry*)(idtRegister.offset+entryOffset*sizeof(IDTEntry)); \
        IDTEntry_SetOffset(interrupt, (uint64_t) handler);                                  \
        interrupt->typeAttribute = attribute;                                               \
        interrupt->selector =  0x08;                                                        \
    }

    SetIDTGate((void*)PageFault_Handler,              0x0E, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)DoubleFault_Handler,            0x08, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)KeyboardInterrupt_Handler,      0x21, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)GeneralProtectionFault_Handler, 0x0E, IDT_TypeAttribute_InterruptGate);

    asm ("lidt %0" : : "m" (idtRegister));
    RemapPIC();
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
    InitHeap((void*)0x0000100000000000, 0x10);
    ISRInstall();

    outportb(PIC1_DATA, 0b11111101);
    outportb(PIC2_DATA, 0b11111111);
    asm ("sti");
  
    static int y = 0;
    TextRenderer_RenderText("Successfully Loaded.", 0, y++);

    void* address1 = malloc(0x8000);
    void* address2 = malloc(16);
    char buffer1[8];
    char buffer2[8];
    itoa((uint64_t)address1, buffer1, 16);
    itoa((uint64_t)address2, buffer2, 16);
    TextRenderer_RenderText(buffer1, 0, y++);
    TextRenderer_RenderText(buffer2, 0, y++);
    free(address1);
    free(address2);

    for (;;);
}
