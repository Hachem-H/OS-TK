#pragma GCC diagnostic ignored "-Wunused-parameter"

#include "Keyboard.h"
#include "Console.h"

#include <stdlib.h>

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

    bool isRead;
    char buffer;
} KeyboardState = {0};

void Keyboard_OnRelease(uint32_t scanCode)
{
}

void Keyboard_OnPress(uint32_t scanCode)
{
    KeyboardState.buffer = KeyboardTranslator(scanCode, KeyboardState.isLeftShift | KeyboardState.isRightShift);
    if (KeyboardState.isRead && KeyboardState.buffer != 0)
    {
        Console_PutChar(KeyboardState.buffer);
        KeyboardState.isRead = false;
    }
}

void KeyboardHandler(uint32_t scanCode)
{
    switch (scanCode)
    {
    case LeftShift:       KeyboardState.isLeftShift  = true;  return;
    case LeftShift+0x80:  KeyboardState.isLeftShift  = false; return;
    case RightShift:      KeyboardState.isRightShift = true;  return;
    case RightShift+0x80: KeyboardState.isRightShift = false; return;
    }

    if ((int)scanCode - 0x80 < 0)
        Keyboard_OnPress(scanCode);
    else 
        Keyboard_OnRelease(scanCode-0x80);
}

char KeyboardTranslator(uint32_t scanCode, bool upper)
{
    if (scanCode > 58)
        return 0;

    if (scanCode == 0x1C)
        return '\n';
    if (scanCode == 0x0E)
        return '\b';

    if (upper)
        return ASCIIKeys[scanCode] - ' ';
    return ASCIIKeys[scanCode];
}

char KeyboardReadChar()
{
    KeyboardState.isRead = true;
    while (KeyboardState.isRead);
    return KeyboardState.buffer;
}