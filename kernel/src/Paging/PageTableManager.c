#include "PageTableManager.h"
#include "PageFrameAllocator.h"

#include <string.h>

PageTableManager GlobalPageTableManager;

void InitPageMapIndex(PageMapIndexer* indexer, uint64_t virtualAddress)
{
    virtualAddress >>= 12;
    indexer->page = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer->pageTable = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer->pageDirectory = virtualAddress & 0x1ff;
    virtualAddress >>= 9;
    indexer->pageDirectoryPointer = virtualAddress & 0x1ff;
}

void PageDirectoryEntry_SetFlag(PageDirectoryEntry* entry, PageDirectoryFlag flag, bool enabled)
{
    uint64_t bitSelector = (uint64_t) 1 << flag;
    entry->value &= ~bitSelector;
    if (enabled)
        entry->value |= bitSelector;
}

void PageDirectoryEntry_SetAddress(PageDirectoryEntry* entry, uint64_t address)
{
    address &= 0x000000ffffffffff;
    entry->value &= 0xfff0000000000fff;
    entry->value |= (address << 12);
}

bool PageDirectoryEntry_GetFlag(PageDirectoryEntry* entry, PageDirectoryFlag flag)
{
    uint64_t bitSelector = (uint64_t) 1 << flag;
    return (entry->value & bitSelector) > 0 ? true : false;
}

uint64_t PageDirectoryEntry_GetAddress(PageDirectoryEntry* entry)
{
    return (entry->value & 0x000ffffffffff000) >> 12;
}

void PageTableManager_MapMemory(void* virtualMemory, void* physicalMemory)
{
    PageMapIndexer indexer;
    InitPageMapIndex(&indexer, (uint64_t) virtualMemory);
    PageDirectoryEntry PDE;

    PageTable* PDP;
    PageTable* PT;
    PageTable* PD;

    PDE = GlobalPageTableManager.pageTableMap4->entries[indexer.pageDirectoryPointer];
    if (!PageDirectoryEntry_GetFlag(&PDE, PageDirectoryFlags_Present))
    {
        PDP = (PageTable*)PageFrameAllocator_RequestPage();
        memset(PDP, 0, 0x1000);
        PageDirectoryEntry_SetAddress(&PDE, (uint64_t)PDP >> 12);
        PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_Present, true);
        PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_ReadWrite, true);
        GlobalPageTableManager.pageTableMap4->entries[indexer.pageDirectoryPointer] = PDE;
    }
    else
        PDP = (PageTable*)((uint64_t)PageDirectoryEntry_GetAddress(&PDE) << 12);
    
    
    PDE = PDP->entries[indexer.pageDirectory];
    if (!PageDirectoryEntry_GetFlag(&PDE, PageDirectoryFlags_Present))
    {
        PD = (PageTable*)PageFrameAllocator_RequestPage();
        memset(PD, 0, 0x1000);
        PageDirectoryEntry_SetAddress(&PDE, (uint64_t)PD >> 12);
        PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_Present, true);
        PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_ReadWrite, true);
        PDP->entries[indexer.pageDirectory] = PDE;
    }
    else
        PD = (PageTable*)((uint64_t)PageDirectoryEntry_GetAddress(&PDE)  << 12);

    PDE = PD->entries[indexer.pageTable];
    if (!PageDirectoryEntry_GetFlag(&PDE, PageDirectoryFlags_Present))
    {
        PT = (PageTable*)PageFrameAllocator_RequestPage();
        memset(PT, 0, 0x1000);
        PageDirectoryEntry_SetAddress(&PDE, (uint64_t)PT >> 12);
        PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_Present, true);
        PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_ReadWrite, true);
        PD->entries[indexer.pageTable] = PDE;
    }
    else
        PT = (PageTable*)((uint64_t)PageDirectoryEntry_GetAddress(&PDE) << 12);

    PDE = PT->entries[indexer.page];
    PageDirectoryEntry_SetAddress(&PDE, (uint64_t)physicalMemory >> 12);
    PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_Present, true);
    PageDirectoryEntry_SetFlag(&PDE, PageDirectoryFlags_ReadWrite, true);
    PT->entries[indexer.page] = PDE;
}
