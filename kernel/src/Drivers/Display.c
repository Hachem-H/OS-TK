#include "Display.h"

static TextRenderer GlobalTextRenderer;

void TextRenderer_InitWith(FrameBuffer* frameBuffer, PSFFont* font)
{
    GlobalTextRenderer.frameBuffer = frameBuffer;
    GlobalTextRenderer.color = 0xFFFFFF;
    GlobalTextRenderer.font = font;
}

void TextRenderer_RenderChar(char character, uint32_t xOff, uint32_t yOff)
{
    FrameBuffer* frameBuffer = GlobalTextRenderer.frameBuffer;
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
    while (*stringPointer != 0x00)
    {
        if (*stringPointer != '\n')
            TextRenderer_RenderChar(*stringPointer, x, y);

        x += 8;
        if(x+8 > GlobalTextRenderer.frameBuffer->width) 
        {
            x *= 0;
            y += 16;
        }

        stringPointer++;
    }
}
void TextRenderer_SetFont(PSFFont* font)                    { GlobalTextRenderer.font = font;               }
void TextRenderer_SetColor(uint32_t color)                  { GlobalTextRenderer.color = color;             }
void TextRenderer_SetFrameBuffer(FrameBuffer* frameBuffer)  { GlobalTextRenderer.frameBuffer = frameBuffer; }

uint32_t TextRenderer_GetWidth()  { return GlobalTextRenderer.frameBuffer->width/8;   }
uint32_t TextRenderer_GetHeight() { return GlobalTextRenderer.frameBuffer->height/16; }