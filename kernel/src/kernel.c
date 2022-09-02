#include "drivers/Display.h"

#include "Memory/PageFrameAllocator.h"
#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"

typedef struct BootInfo_t
{
	FrameBuffer* frameBuffer;
	PSFFont* font;

	EFI_MEMORY_DESCRIPTOR* memoryMap;
	uint64_t memoryMapSize;
	uint64_t descriptorSize;
} BootInfo;

extern uint64_t KernelStart;
extern uint64_t KernelEnd;

void _start(BootInfo* bootInfo)
{
    TextRenderer_InitWith(bootInfo->frameBuffer, bootInfo->font);

	PageFrameAllocator pageFrameAllocator;
	PageFrameAllocator_ReadEFIMemoryMap(&pageFrameAllocator, bootInfo->memoryMap, bootInfo->memoryMapSize, bootInfo->descriptorSize);

	uint64_t kernelSize = &KernelEnd - &KernelStart;
	uint64_t kernelPages = kernelSize/4096+1;
	PageFrameAllocator_LockPages(&pageFrameAllocator, &KernelStart, kernelPages);

	TextRenderer_RenderText("Free RAM: ");
	TextRenderer_RenderNumber(PageFrameAllocator_GetFreeRAM()/1024);
	TextRenderer_RenderText(" KB\n");
	TextRenderer_RenderText("Used RAM: ");
	TextRenderer_RenderNumber(PageFrameAllocator_GetUsedRAM()/1024);
	TextRenderer_RenderText(" KB\n");
	TextRenderer_RenderText("Reserved RAM: ");
	TextRenderer_RenderNumber(PageFrameAllocator_GetReservedRAM()/1024);
	TextRenderer_RenderText(" KB\n\n");

	for (int i = 0; i < 20; ++i)
	{
		void* address = PageFrameAllocator_RequestPage(&pageFrameAllocator);
		TextRenderer_RenderHex64((uint64_t)address);
		TextRenderer_RenderText("\n");
	}
}
