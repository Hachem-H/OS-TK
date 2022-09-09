#include "ACPI.h"

void* ACPI_FindTable(ACPI_SDTHeader* header, char* signature)
{
    int entries = (header->size - sizeof(ACPI_SDTHeader)) / 8;

    for (int i = 0; i < entries; ++i)
    {
        ACPI_SDTHeader* sdtHeader = (ACPI_SDTHeader*)*(uint64_t*)((uint64_t) header + 
                                                     sizeof(ACPI_SDTHeader) + (i*8));

        for (int j = 0; j < 4; ++j)
        { 
            if (sdtHeader->signature[i] != signature[i])
                break;
            if (i == 3)
                return sdtHeader;
        }
    }

    return 0;
}
