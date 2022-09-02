#include "drivers/Display.h"

#include "Memory/EFIMemory.h"
#include "Memory/Bitmap.h"
#include "Memory/Memory.h"

typedef struct BootInfo_t
{
	FrameBuffer* frameBuffer;
	PSFFont* font;

	EFI_MEMORY_DESCRIPTOR* memoryMap;
	uint64_t memoryMapSize;
	uint64_t  descriptorSize;
} BootInfo;

void _start(BootInfo* bootInfo)
{
    TextRenderer_InitWith(bootInfo->frameBuffer, bootInfo->font);
}
