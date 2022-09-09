#pragma once

#include <stdint.h>

typedef struct ACPI_RSDP2_t
{
    uint8_t signature[8];
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t revision;
    uint32_t RSDTAddress;
    uint32_t size;
    uint64_t XSDTAddress;
    uint8_t extendedChecksum;
    uint8_t reserved[3];
} __attribute__((packed)) ACPI_RSDP2;

typedef struct ACPI_SDTHeader_t
{
    uint8_t signature[4];
    uint32_t size;
    uint8_t revision;
    uint8_t checksum;
    uint8_t OEMID[6];
    uint8_t OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t creatorID;
    uint32_t creatorRevision;
} __attribute__((packed)) ACPI_SDTHeader;

typedef struct ACPI_MCFGHeader_t
{
    ACPI_SDTHeader header;
    uint64_t reserved;
} __attribute__((packed)) ACPI_MCFGHeader;

typedef struct ACPI_DeviceConfig_t
{
    uint64_t baseAddress;
    uint16_t PCISegmentGroup;
    uint8_t startBUS;
    uint8_t endBUS;
    uint32_t reserved;
} __attribute__((packed)) ACPI_DeviceConfig;

void* ACPI_FindTable(ACPI_SDTHeader* header, char* signature);
