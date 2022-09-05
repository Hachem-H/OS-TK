#pragma once

#include <stdint.h>
#include <stdbool.h>

void Keyboard_OnRelease(uint32_t scanCode);
void Keyboard_OnPress(uint32_t scanCode);
void KeyboardHandler(uint32_t scanCode);

char KeyboardTranslator(uint32_t scanCode, bool upper);