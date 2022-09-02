#include "drivers/Display.h"
#include "EFI/EFIMemory.h"

typedef struct BootInfo_t
{
	FrameBuffer* frameBuffer;
	PSFFont* font;

	void* memoryMap;
	uint64_t memoryMapSize;
	uint64_t  descriptorSize;
} BootInfo;

static const char* ToString(uint64_t value)
{
    static char output[128];
    uint8_t size;
    uint64_t tester = value;
    
    while (tester / 10 > 0)
    {
        tester /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        output[size - index] = remainder + '0';
        index++;
    }
    
    uint8_t remainder = value % 10;
    output[size - index] = remainder + '0';
    output[size + 1] = 0; 
    return output;
}

void _start(BootInfo* bootInfo)
{
    TextRenderer_InitWith(bootInfo->frameBuffer, bootInfo->font);

    for (int i = 0; i < bootInfo->memoryMapSize / bootInfo->descriptorSize; ++i)
    {
        EFI_MEMORY_DESCRIPTOR* descriptor = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)bootInfo->memoryMap + 
                                                                     (i * bootInfo->descriptorSize));
        TextRenderer_RenderText(EFI_MEMORY_TYPES[descriptor->type]);
        TextRenderer_RenderText(": ");
        TextRenderer_SetColor(0x39D6AB);
        TextRenderer_RenderText(ToString(descriptor->pagesAmount * 4096/1024));
        TextRenderer_RenderText(" KB\n");
        TextRenderer_SetColor(0xFFFFFF);
    }
}
