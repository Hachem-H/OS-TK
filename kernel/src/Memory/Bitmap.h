#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool Bitmap_Get(uint8_t* buffer, uint64_t index);
bool Bitmap_Set(uint8_t* buffer, uint64_t index, bool value);