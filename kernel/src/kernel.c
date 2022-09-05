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

static void InstallInterrupts()
{
    Kernel.idtRegister.limit = 0x0FFF;
    Kernel.idtRegister.offset = (uint64_t)PageFrameAllocator_RequestPage();

    IDTEntry* pageFaultInterrupt = (IDTEntry*)(Kernel.idtRegister.offset + 0xE * sizeof(IDTEntry));
    IDTEntry_SetOffset(pageFaultInterrupt, (uint64_t)PageFault_Handler);
    pageFaultInterrupt->typeAttribute = IDT_TypeAttribute_InterruptGate;
    pageFaultInterrupt->selector = 0x08;

    IDTEntry* doubleFaultInterrupt = (IDTEntry*)(Kernel.idtRegister.offset + 0x8 * sizeof(IDTEntry));
    IDTEntry_SetOffset(doubleFaultInterrupt, (uint64_t)DoubleFault_Handler);
    doubleFaultInterrupt->typeAttribute = IDT_TypeAttribute_InterruptGate;
    doubleFaultInterrupt->selector = 0x08;

    IDTEntry* GPFaultInterrupt = (IDTEntry*)(Kernel.idtRegister.offset + 0xD * sizeof(IDTEntry));
    IDTEntry_SetOffset(GPFaultInterrupt, (uint64_t)GeneralProtectionFault_Handler);
    GPFaultInterrupt->typeAttribute = IDT_TypeAttribute_InterruptGate;
    GPFaultInterrupt->selector = 0x08;

    IDTEntry* keyboardInterrupt = (IDTEntry*)(Kernel.idtRegister.offset + 0x21 * sizeof(IDTEntry));
    IDTEntry_SetOffset(keyboardInterrupt, (uint64_t)GeneralProtectionFault_Handler);
    keyboardInterrupt->typeAttribute = IDT_TypeAttribute_InterruptGate;
    keyboardInterrupt->selector = 0x08;

    asm ("lidt %0" : : "m" (Kernel.idtRegister));

    RemapPIC();
    outportb(PIC1_DATA, 0b11111101);
    outportb(PIC2_DATA, 0b11111111);

    asm ("sti");
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
    //InstallInterrupts();
    
    FrameBuffer_ClearColor(0xFFFFFF);
    for (;;);
}
