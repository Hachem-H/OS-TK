#pragma once

#include <stdint.h>

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

typedef struct FrameBuffer_t
{
    void* baseAddress;
	unsigned long long bufferSize;

	uint32_t width;
	uint32_t height;
	uint32_t pixelsPerScanLine;
} FrameBuffer;

typedef struct TextRenderer_t
{
    struct 
    {
        uint32_t x;
        uint32_t y;
    } cursorPosition;

    FrameBuffer* frameBuffer;
    uint32_t color;
    PSFFont* font;
} TextRenderer;

void TextRenderer_InitWith(FrameBuffer* frameBuffer, PSFFont* font);
void TextRenderer_RenderChar(char character, uint32_t xOff, uint32_t yOff);
void TextRenderer_RenderText(const char* string);

void TextRenderer_SetFont(PSFFont* font);
void TextRenderer_SetColor(uint32_t color);
void TextRenderer_SetFrameBuffer(FrameBuffer* frameBuffer);
void TextRenderer_SetCursorPosition(uint32_t x, uint32_t y);