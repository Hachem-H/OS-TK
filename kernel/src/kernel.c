#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"
#include "Memory/Heap.h"

#include "Descriptors/GDT.h"
#include "Descriptors/IDT.h"

#include "Drivers/Display.h"
#include "Drivers/Console.h"
#include "Drivers/PIC.h"
#include "Drivers/PIT.h"

#include "Core/Interrupts.h"
#include "Core/ACPI.h"
#include "Core/PCI.h"
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

    SetIDTGate((void*)DivisionByZero_Handler,            0x00, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)Debug_Handler,                     0x01, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)NonMaskableInterrupt_Handler,      0x02, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)Breakpoint_Handler,                0x03, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)IntoDetectedOverflow_Handler,      0x04, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)OutofBounds_Handler,               0x05, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)InvalidOpcode_Handler,             0x06, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)NoCoprocessor_Handler,             0x07, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)DoubleFault_Handler,               0x08, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)CoprocessorSegmentOverrun_Handler, 0x09, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)BadTSS_Handler,                    0x0A, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)SegmentNotPresent_Handler,         0x0B, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)StackFault_Handler,                0x0C, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)GeneralProtectionFault_Handler,    0x0D, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)PageFault_Handler,                 0x0E, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)UnknownInterrupt_Handler,          0x0F, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)CoprocessorFault_Handler,          0x10, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)AlignmentCheck_Handler,            0x12, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)MachineCheck_Handler,              0x13, IDT_TypeAttribute_InterruptGate);

    SetIDTGate((void*)KeyboardInterrupt_Handler, 0x21, IDT_TypeAttribute_InterruptGate);
    SetIDTGate((void*)PITInterrupt_Handler,      0x20, IDT_TypeAttribute_InterruptGate);

    asm ("lidt %0" : : "m" (idtRegister));
    RemapPIC();
}

static void InitACPI(ACPI_RSDP2* rsdp)
{
    ACPI_SDTHeader*  xsdt = (ACPI_SDTHeader*)(rsdp->XSDTAddress);
    ACPI_MCFGHeader* mcfg = (ACPI_MCFGHeader*)ACPI_FindTable(xsdt, (char*)"MCFG");
    EnumeratePCI(mcfg);
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

    Console_Init();

    ISRInstall();
    //InitACPI((ACPI_RSDP2*)bootInfo->rsdp);

    outportb(PIC1_DATA, 0b11111000);
    outportb(PIC2_DATA, 0b11101111);
    asm ("sti");

    kmain();
    for (;;);
}
