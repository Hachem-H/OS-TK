#include "Display.h"

#include <string.h>

static TextRenderer GlobalTextRenderer;
static FrameBuffer* GlobalFrameBuffer;

void TextRenderer_Init(PSFFont* font)
{
    GlobalTextRenderer.color = 0xFFFFFF;
    GlobalTextRenderer.font = font;
}

void FrameBuffer_Init(FrameBuffer* frameBuffer)
{
    GlobalFrameBuffer = frameBuffer;
    FrameBuffer_ClearColor(0x00);
}

void FrameBuffer_ClearColor(uint32_t color)
{
    uint64_t frameBufferBase = (uint64_t)GlobalFrameBuffer->baseAddress;
    uint64_t bytesPerScanline = GlobalFrameBuffer->pixelsPerScanLine * 4;
    uint64_t frameBufferHeight = GlobalFrameBuffer->height;

    for (uint64_t verticalScanline = 0; verticalScanline < frameBufferHeight; verticalScanline ++)
    {
        uint64_t pixelPointerBase = frameBufferBase + (bytesPerScanline * verticalScanline);
        for (uint32_t* pixelPointer = (uint32_t*)pixelPointerBase;
             pixelPointer < (uint32_t*)(pixelPointerBase + bytesPerScanline); 
             pixelPointer++)
            *pixelPointer = color;
    }
}

void TextRenderer_RenderChar(char character, uint32_t xOff, uint32_t yOff)
{
    FrameBuffer* frameBuffer = GlobalFrameBuffer;
    PSFFont* font = GlobalTextRenderer.font;

    unsigned int* pixelPointer = (unsigned int*) frameBuffer->baseAddress;
    char* fontPointer = (char*)font->glyphBuffer+character*font->header->charSize;

    for (unsigned long y = yOff; y < yOff+16; ++y)
    {
        for (unsigned long x = xOff;  x < xOff+8; ++x)
            if ((*fontPointer & (0b10000000 >> (x-xOff))) > 0)
                pixelPointer[x+y*frameBuffer->pixelsPerScanLine] = GlobalTextRenderer.color;

        fontPointer++;
    }
}

void TextRenderer_RenderText(const char* string, uint32_t x, uint32_t y)
{
    const char* stringPointer = string;
    x *= 8;
    y *= 16;
    
    while (*stringPointer != 0x00)
    {
        if (*stringPointer != '\n')
            TextRenderer_RenderChar(*stringPointer, x, y);

        x += 8;
        if(x+8 > GlobalFrameBuffer->width) 
        {
            x *= 0;
            y += 16;
        }

        stringPointer++;
    }
}
void TextRenderer_SetFont(PSFFont* font)    { GlobalTextRenderer.font = font;   }
void TextRenderer_SetColor(uint32_t color)  { GlobalTextRenderer.color = color; }

uint32_t TextRenderer_GetWidth()  { return GlobalFrameBuffer->width/8;   }
uint32_t TextRenderer_GetHeight() { return GlobalFrameBuffer->height/16; }