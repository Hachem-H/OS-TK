#include "EFIMemory.h"

const char* EFI_MEMORY_TYPES[] = 
{
    "EFIReservedMemoryType",
    "EFILoaderCode",
    "EFILoaderData",
    "EFIBootServicesCode",
    "EFIBootServicesData",
    "EFIRuntimeServicesCode",
    "EFIRuntimeServicesData",
    "EFIConventionalMemory",
    "EFIUnusableMemory",
    "EFIACPIReclaimMemory",
    "EFIACPIMemoryNVS",
    "EFIMemoryMappedIO",
    "EFIMemoryMappedIOPortSpace",
    "EFIPalCode",  
};