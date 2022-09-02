#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct Bitmap_t
{
    uint8_t* buffer;
    uint64_t size;
} Bitmap;

bool Bitmap_Get(Bitmap* bitmap, uint64_t index);
bool Bitmap_Set(Bitmap* bitmap, uint64_t index, bool value);