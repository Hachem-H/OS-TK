#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct PageMapIndexer_t
{
    uint64_t pageDirectoryPointer;
    uint64_t pageDirectory;
    uint64_t pageTable;
    uint64_t page;
} PageMapIndexer;

typedef struct PageDirectoryEntry_t
{
    bool present       : 1;
    bool readWrite     : 1;
    bool adminUser     : 1;
    bool writeThrough  : 1;
    bool cacheDisabled : 1;
    bool accessed      : 1;
    bool ignore0       : 1; 
    bool largerPages   : 1;
    bool ingore1       : 1;

    uint8_t available  : 3;
    uint64_t address   : 52;
} PageDirectoryEntry;

typedef struct PageTable_t
{
    PageDirectoryEntry entries [512];
} __attribute__((aligned(0x1000))) PageTable;

typedef struct PageTableManager_t
{
    PageTable* pageTableMap4;
} PageTableManager;

void InitPageMapIndex(PageMapIndexer* indexer, uint64_t virtualAddress);
void PageTableManager_MapMemory(PageTableManager* pageTableManager, void* virtualMemory, void* physicalMemory);