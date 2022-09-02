#pragma once

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

	unsigned int width;
	unsigned int height;
	unsigned int pixelsPerScanLine;
} FrameBuffer;

typedef struct TextRenderer_t
{
    struct 
    {
        unsigned int x;
        unsigned int y;
    } cursorPosition;

    FrameBuffer* frameBuffer;
    unsigned int color;
    PSFFont* font;
} TextRenderer;

static TextRenderer GlobalTextRenderer;
void TextRenderer_InitWith(FrameBuffer* frameBuffer, PSFFont* font);
void TextRenderer_RenderChar(char character, unsigned int xOff, unsigned int yOff);
void TextRenderer_RenderText(const char* string);
