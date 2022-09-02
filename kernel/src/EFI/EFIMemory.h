#pragma once

#include <stdint.h>

typedef struct EFI_MEMORY_DESCRIPTOR_t
{
    uint32_t type;

    void* physicalAddress;
    void* virtualAddress;

    uint64_t pagesAmount;
    uint64_t attributes;
} EFI_MEMORY_DESCRIPTOR;

extern const char* EFI_MEMORY_TYPES[];