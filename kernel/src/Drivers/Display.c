#include "Display.h"

static TextRenderer GlobalTextRenderer;

void TextRenderer_InitWith(FrameBuffer* frameBuffer, PSFFont* font)
{
    GlobalTextRenderer.cursorPosition.x = 0;
    GlobalTextRenderer.cursorPosition.y = 0;

    GlobalTextRenderer.frameBuffer = frameBuffer;
    GlobalTextRenderer.color = 0xFFFFFF;
    GlobalTextRenderer.font = font;
}

void TextRenderer_RenderChar(char character, unsigned int xOff, unsigned int yOff)
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

void TextRenderer_RenderNumber(int64_t value)
{
    static char output[128];
    uint8_t isNegative = 0;

    if (value < 0)
    {
        isNegative =  1;
        value     *= -1;
        output[0]  = '-';
    }

    uint8_t size;
    uint64_t sizeTest = value;

    while (sizeTest / 10 > 0)
    {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while(value / 10 > 0)
    {
        uint8_t remainder = value % 10;
        value /= 10;
        output[isNegative+size-index] = remainder + '0';
        index++;
    }
    
    uint8_t remainder = value % 10;
    output[isNegative+size-index] = remainder + '0';
    output[isNegative+size+1] = 0; 
    TextRenderer_RenderText(output);
}

void TextRenderer_RenderText(const char* string)
{
    const char* stringPointer = string;
    while (*stringPointer != 0x00)
    {
        if (*stringPointer != '\n')
            TextRenderer_RenderChar(*stringPointer, GlobalTextRenderer.cursorPosition.x,
                                                    GlobalTextRenderer.cursorPosition.y);

        GlobalTextRenderer.cursorPosition.x += 8;
        if(GlobalTextRenderer.cursorPosition.x+8 > GlobalTextRenderer.frameBuffer->width ||
           *stringPointer == '\n') 
        {
            GlobalTextRenderer.cursorPosition.x *= 0;
            GlobalTextRenderer.cursorPosition.y += 16;
        }

        stringPointer++;
    }
}

void TextRenderer_RenderHex64(uint64_t value)
{
    char output[128];
    uint64_t* valuePointer = &value;
    uint8_t* pointer;
    uint8_t  temporary;
    uint8_t  size = 8 * 2 - 1;
    
    for (uint8_t i = 0; i < size; i++)
    {
        pointer = ((uint8_t*)valuePointer + i);
        temporary = ((*pointer & 0xF0) >> 4);
        output[size-(i*2+1)] = temporary + (temporary > 9 ? 55 : '0');
        temporary = ((*pointer & 0x0F));
        output[size-(i*2)]   = temporary + (temporary > 9 ? 55 : '0');
    }

    output[size + 1] = 0;

    TextRenderer_RenderText("0x");
    TextRenderer_RenderText(output);
}

void TextRenderer_RenderHex32(uint32_t value)
{
    char output[128];
    uint32_t* valuePointer = &value;
    uint8_t* pointer;
    uint8_t  temporary;
    uint8_t  size = 4 * 2 - 1;
    
    for (uint8_t i = 0; i < size; i++)
    {
        pointer = ((uint8_t*)valuePointer + i);
        temporary = ((*pointer & 0xF0) >> 4);
        output[size-(i*2+1)] = temporary + (temporary > 9 ? 55 : '0');
        temporary = ((*pointer & 0x0F));
        output[size-(i*2)]   = temporary + (temporary > 9 ? 55 : '0');
    }

    output[size + 1] = 0;

    TextRenderer_RenderText("0x");
    TextRenderer_RenderText(output);
}

void TextRenderer_RenderHex16(uint16_t value)
{
    char output[128];
    uint16_t* valuePointer = &value;
    uint8_t* pointer;
    uint8_t  temporary;
    uint8_t  size = 2 * 2 - 1;
    
    for (uint8_t i = 0; i < size; i++)
    {
        pointer = ((uint8_t*)valuePointer + i);
        temporary = ((*pointer & 0xF0) >> 4);
        output[size-(i*2+1)] = temporary + (temporary > 9 ? 55 : '0');
        temporary = ((*pointer & 0x0F));
        output[size-(i*2)]   = temporary + (temporary > 9 ? 55 : '0');
    }

    output[size + 1] = 0;

    TextRenderer_RenderText("0x");
    TextRenderer_RenderText(output);
}

void TextRenderer_SetFont(PSFFont* font)                    { GlobalTextRenderer.font = font;               }
void TextRenderer_SetColor(uint32_t color)                  { GlobalTextRenderer.color = color;             }
void TextRenderer_SetFrameBuffer(FrameBuffer* frameBuffer)  { GlobalTextRenderer.frameBuffer = frameBuffer; }
void TextRenderer_SetCursorPosition(uint32_t x, uint32_t y) { GlobalTextRenderer.cursorPosition.x = x; 
                                                              GlobalTextRenderer.cursorPosition.y = y;      }