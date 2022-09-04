#pragma once

#include "stdint.h"

#define IDT_TypeAttribute_InterruptGate 0b10001110
#define IDT_TypeAttribute_CallGate      0b10001100
#define IDT_TypeAttribute_TrapGate      0b10001111

typedef struct IDTEntry_t
{
    uint16_t offset0; 
    uint16_t selector;  
    uint8_t interruptStackTable;
    uint8_t typeAttribute;
    uint16_t offset1;
    uint32_t offset2;
    uint32_t ignore;
} IDTEntry;

typedef struct IDTRegister_t
{
    uint64_t limit;
    uint64_t offset;
} __attribute__((packed)) IDTRegister;

void IDTEntry_SetOffset(IDTEntry* entry, uint64_t offset);
uint64_t IDTEntry_GetOffset(IDTEntry* entry);