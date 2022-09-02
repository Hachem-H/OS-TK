#include "Bitmap.h"

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

bool Bitmap_Get(Bitmap* bitmap, uint64_t index)
{
    if (index > bitmap->size*8)
        return false;

    uint64_t byteIndex  = index / 8;
    uint8_t  bitIndex   = index % 8;
    uint8_t  bitIndexer = 0b10000000 >> bitIndex;

    if ((bitmap->buffer[byteIndex] & bitIndexer) > 0)
        return true;
    return false;
}

bool Bitmap_Set(Bitmap* bitmap, uint64_t index, bool value)
{
    if (index > bitmap->size*8)
        return false;
        
    uint64_t byteIndex  = index / 8;
    uint8_t  bitIndex   = index % 8;
    uint8_t  bitIndexer = 0b10000000 >> bitIndex;

    bitmap->buffer[byteIndex] &= ~bitIndexer;
    if (value)
        bitmap->buffer[byteIndex] |= bitIndexer;
    return true;
}