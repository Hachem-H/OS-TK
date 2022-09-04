#include "GDT.h"

__attribute__((aligned(0x1000))) 
GDT GlobalDescriptorTable = 
{
    (GDTEntry) {0, 0, 0, 0x00, 0x00, 0},
    (GDTEntry) {0, 0, 0, 0x9A, 0xA0, 0},
    (GDTEntry) {0, 0, 0, 0x92, 0xA0, 0},
    (GDTEntry) {0, 0, 0, 0x00, 0x00, 0},
    (GDTEntry) {0, 0, 0, 0x9A, 0xA0, 0},
    (GDTEntry) {0, 0, 0, 0x92, 0xA0, 0},
};