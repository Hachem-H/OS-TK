#pragma once

typedef unsigned long long size_t;

typedef struct FrameBuffer_t
{
    void* baseAddress;
	size_t bufferSize;

	unsigned int width;
	unsigned int height;
	unsigned int pixelsPerScanLine;
} FrameBuffer;

typedef struct PSFHeader_t 
{
	unsigned char magic[2];
	unsigned char mode;
	unsigned char charSize;
} PSFHeader;

typedef struct PSFFont_t 
{
	PSFHeader* header;
	void* glyphBuffer;
} PSFFont;
