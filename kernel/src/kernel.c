#include "Drivers/Display.h"

#include "Paging/PageFrameAllocator.h"
#include "Paging/PageTableManager.h"

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"

#include "Common/BootInfo.h"

extern uint64_t KernelStart;
extern uint64_t KernelEnd;

extern void kmain();
void _start(BootInfo* bootInfo)
{
    TextRenderer_InitWith(bootInfo->frameBuffer, bootInfo->font);
    kmain();
}
