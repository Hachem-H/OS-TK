#include "PageTableManager.h"
#include "PageFrameAllocator.h"

#include <string.h>

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

void PageTableManager_MapMemory(PageTableManager* pageTableManager, void* virtualMemory, void* physicalMemory)
{
    PageTable* pageDirectoryPointer;
    PageTable* pageDirectory;
    PageTable* pageTable;

    PageMapIndexer indexer;
    PageDirectoryEntry pageDirectoryEntry;
    InitPageMapIndex(&indexer, (uint64_t)virtualMemory);

    pageDirectoryEntry = pageTableManager->pageTableMap4->entries[indexer.pageDirectoryPointer];
    if (!pageDirectoryEntry.present)
    {
        pageDirectoryPointer = (PageTable*)PageFrameAllocator_RequestPage();
        memset(pageDirectoryPointer, 0, 0x1000);
        pageDirectoryEntry.address = (uint64_t)pageDirectoryPointer >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pageTableManager->pageTableMap4->entries[indexer.pageDirectoryPointer] = pageDirectoryEntry;
    }
    else
        pageDirectoryPointer = (PageTable*)((uint64_t)pageDirectoryEntry.address << 12);
    
    
    pageDirectoryEntry = pageDirectoryPointer->entries[indexer.pageDirectory];
    if (!pageDirectoryEntry.present)
    {
        pageDirectory = (PageTable*)PageFrameAllocator_RequestPage();
        memset(pageDirectory, 0, 0x1000);
        pageDirectoryEntry.address = (uint64_t)pageDirectory >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pageDirectoryPointer->entries[indexer.pageDirectory] = pageDirectoryEntry;
    }
    else
        pageDirectory = (PageTable*)((uint64_t)pageDirectoryEntry.address << 12);

    pageDirectoryEntry = pageDirectory->entries[indexer.pageTable];
    if (!pageDirectoryEntry.present)
    {
        pageTable = (PageTable*)PageFrameAllocator_RequestPage();
        memset(pageTable, 0, 0x1000);
        pageDirectoryEntry.address = (uint64_t)pageTable >> 12;
        pageDirectoryEntry.present = true;
        pageDirectoryEntry.readWrite = true;
        pageDirectory->entries[indexer.pageTable] = pageDirectoryEntry;
    }
    else
        pageTable = (PageTable*)((uint64_t)pageDirectoryEntry.address << 12);

    pageDirectoryEntry = pageTable->entries[indexer.page];
    pageDirectoryEntry.address = (uint64_t)physicalMemory >> 12;
    pageDirectoryEntry.present = true;
    pageDirectoryEntry.readWrite = true;
    pageTable->entries[indexer.page] = pageDirectoryEntry;
}
