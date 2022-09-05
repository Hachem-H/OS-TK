#pragma once

#include "stdint.h"

typedef struct GDTDescriptor_t 
{
    uint16_t size;
    uint64_t offset;
} __attribute__((packed)) GDTDescriptor;

typedef struct GDTEntry_t 
{
    uint16_t limit0;
    uint16_t base0;
    uint8_t base1;
    uint8_t accessByte;
    uint8_t flags;
    uint8_t base2;
} __attribute__((packed)) GDTEntry;

typedef struct GDT_t
{
    GDTEntry null;
    GDTEntry kernelCode;
    GDTEntry kernelData;
    GDTEntry userNull;
    GDTEntry userCode;
    GDTEntry userData;
} __attribute__((packed)) 
  __attribute__((aligned(0x1000))) GDT;

extern GDT GlobalDescriptorTable;
extern void LoadGDT(GDTDescriptor* gdtDescriptor);