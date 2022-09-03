#pragma once

typedef struct BootInfo_t
{
	FrameBuffer* frameBuffer;
	PSFFont* font;

	EFI_MEMORY_DESCRIPTOR* memoryMap;
	uint64_t memoryMapSize;
	uint64_t descriptorSize;
} BootInfo;