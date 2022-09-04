#pragma once

#include <stdint.h>
#include "EFIMemory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* memoryMap, uint64_t memoryMapEntries, uint64_t descriptorSize);
