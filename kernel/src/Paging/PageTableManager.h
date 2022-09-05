#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef enum PageDirectoryFlag_t
{
    PageDirectoryFlags_Present       =  0,
    PageDirectoryFlags_ReadWrite     =  1,
    PageDirectoryFlags_UserSuper     =  2,
    PageDirectoryFlags_WriteThroug   =  3,
    PageDirectoryFlags_CacheDisabled =  4,
    PageDirectoryFlags_Accessed      =  5,
    PageDirectoryFlags_LargerPages   =  7,
    PageDirectoryFlags_Custom0       =  9,
    PageDirectoryFlags_Custom1       = 10,
    PageDirectoryFlags_Custom2       = 11,
    PageDirectoryFlags_NX = 63,
} PageDirectoryFlag;

typedef struct PageMapIndexer_t
{
    uint64_t pageDirectoryPointer;
    uint64_t pageDirectory;
    uint64_t pageTable;
    uint64_t page;
} PageMapIndexer;

typedef struct PageDirectoryEntry_t
{
    uint64_t value;
} PageDirectoryEntry;

typedef struct PageTable_t
{
    PageDirectoryEntry entries[512];
} __attribute__((aligned(0x1000))) PageTable;

typedef struct PageTableManager_t
{
    PageTable* pageTableMap4;
} PageTableManager;

void     PageDirectoryEntry_SetFlag(PageDirectoryEntry* entry, PageDirectoryFlag flag, bool enabled);
void     PageDirectoryEntry_SetAddress(PageDirectoryEntry* entry, uint64_t address);
bool     PageDirectoryEntry_GetFlag(PageDirectoryEntry* entry, PageDirectoryFlag flag);
uint64_t PageDirectoryEntry_GetAddress(PageDirectoryEntry* entry);

void InitPageMapIndex(PageMapIndexer* indexer, uint64_t virtualAddress);
void PageTableManager_MapMemory(PageTableManager* pageTableManager, void* virtualMemory, void* physicalMemory);
