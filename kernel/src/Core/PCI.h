#pragma once

#include "ACPI.h"

typedef struct PCI_DeviceHeader_t
{
    uint16_t vendorID;
    uint16_t deviceID;
    uint16_t command;
    uint16_t status;

    uint8_t revisionID;
    uint8_t progInterfaceByte;
    uint8_t subclass;
    uint8_t class;
    uint8_t cacheLineSize;
    uint8_t latencyTimer;
    uint8_t headerType;
    uint8_t BIST;
} PCI_DeviceHeader;

void EnumeratePCI(ACPI_MCFGHeader* mcfg);

const char* PCI_GetVendorName(uint16_t vendorID);
const char* PCI_GetDeviceName(uint16_t vendorID, uint16_t deviceID);
const char* PCI_GetSubclassName(uint8_t classCode, uint8_t subclassCode);
const char* PCI_GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF);
