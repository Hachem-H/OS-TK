#include "Bitmap.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool Bitmap_Get(uint8_t* buffer, uint64_t index)
{
    uint64_t byteIndex  = index / 8;
    uint8_t  bitIndex   = index % 8;
    uint8_t  bitIndexer = 0b10000000 >> bitIndex;

    if ((buffer[byteIndex] & bitIndexer) > 0)
        return true;
    return false;
}

bool Bitmap_Set(uint8_t* buffer, uint64_t index, bool value)
{
    uint64_t byteIndex  = index / 8;
    uint8_t  bitIndex   = index % 8;
    uint8_t  bitIndexer = 0b10000000 >> bitIndex;

    buffer[byteIndex] &= ~bitIndexer;
    if (value)
        buffer[byteIndex] |= bitIndexer;
    return true;
}