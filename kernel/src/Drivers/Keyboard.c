#include "Keyboard.h"
#include "Display.h"
#include "stdlib.h"

#define LeftShift  0x2A
#define RightShift 0x36

static const char ASCIIKeys[] =
{
     0 ,  0  , '1', '2', '3', '4', '5', '6', '7', '8', '9', '0' , '-', '=', 0, 
     0 , 'q' , 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[' , ']',  0 , 
     0 , 'a' , 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 
     0 , '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/' ,  0 ,
    '*',  0  , ' ',
};

static struct KeyboardState_t
{
    bool isLeftShift;
    bool isRightShift;
} KeyboardState = {0};

void Keyboard_OnRelease(uint32_t scanCode)
{
    char character = KeyboardTranslator(scanCode, KeyboardState.isLeftShift | KeyboardState.isRightShift);
    TextRenderer_RenderText("Keyboard OnRelease, character = ", 0, 0);
    if (character) 
        TextRenderer_RenderChar(character, 32, 0);
}

void Keyboard_OnPress(uint32_t scanCode)
{
    char character = KeyboardTranslator(scanCode, KeyboardState.isLeftShift | KeyboardState.isRightShift);
    TextRenderer_RenderText("Keyboard OnPress, character = ", 0, 0);
    if (character) 
        TextRenderer_RenderChar(character, 30, 0);
}

void KeyboardHandler(uint32_t scanCode)
{
    switch (scanCode)
    {
    case LeftShift:       KeyboardState.isLeftShift  = true;  break;
    case LeftShift+0x80:  KeyboardState.isLeftShift  = false; break;
    case RightShift:      KeyboardState.isRightShift = true;  break;
    case RightShift+0x80: KeyboardState.isRightShift = false; break;
    }

    FrameBuffer_ClearColor(0);
    if ((int)scanCode - 0x80 < 0)
        Keyboard_OnPress(scanCode);
    else 
        Keyboard_OnRelease(scanCode-0x80);
}

char KeyboardTranslator(uint32_t scanCode, bool upper)
{
    if (scanCode > 58)
        return 0;
    if (upper)
        return ASCIIKeys[scanCode] - ' ';
    return ASCIIKeys[scanCode];
}