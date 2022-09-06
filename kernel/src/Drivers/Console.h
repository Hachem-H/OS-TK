#pragma once

#include <stdint.h>

void Console_Init();
void Console_Destroy();

void Console_Clear();
void Console_NewLine();
void Console_PutChar(const char character);
void Console_PrintString(const char* string);

void Console_SetPosition(uint32_t x, uint32_t y);