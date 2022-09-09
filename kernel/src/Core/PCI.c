#include <stdint.h>
#include <stdlib.h>

#include "Paging/PageTableManager.h"
#include "Drivers/Console.h"
#include "PCI.h"

static const char* MassStorageControllerName(uint8_t code)
{
    switch (code)
    {
        case 0x00: return "SCSI Bus Controller";
        case 0x01: return "IDE Controller";
        case 0x02: return "Floppy Disk Controller";
        case 0x03: return "IPI Bus Controller";
        case 0x04: return "RAID Controller";
        case 0x05: return "ATA Controller";
        case 0x06: return "Serial ATA";
        case 0x07: return "Serial Attached SCSI";
        case 0x08: return "Non-Volatile Memory Controller";
        case 0x80: return "Other";
    }

    static char buffer[24];
    itoa(code, buffer, 16);
    return buffer;
}

static const char* SerialBusControllerName(uint8_t code)
{
    switch (code)
    {
        case 0x00: return "FireWire (IEEE 1394) Controller";
        case 0x01: return "ACCESS Bus";
        case 0x02: return "SSA";
        case 0x03: return "USB Controller";
        case 0x04: return "Fibre Channel";
        case 0x05: return "SMBus";
        case 0x06: return "Infiniband";
        case 0x07: return "IPMI Interface";
        case 0x08: return "SERCOS Interface (IEC 61491)";
        case 0x09: return "CANbus";
        case 0x80: return "SerialBusController - Other";
    }

    static char buffer[24];
    itoa(code, buffer, 16);
    return buffer;
}

static const char* BridgeDeviceName(uint8_t code)
{
    switch (code)
    {
        case 0x00: return "Host Bridge";
        case 0x01: return "ISA Bridge";
        case 0x02: return "EISA Bridge";
        case 0x03: return "MCA Bridge";
        case 0x04: return "PCI-to-PCI Bridge";
        case 0x05: return "PCMCIA Bridge";
        case 0x06: return "NuBus Bridge";
        case 0x07: return "CardBus Bridge";
        case 0x08: return "RACEway Bridge";
        case 0x09: return "PCI-to-PCI Bridge";
        case 0x0a: return "InfiniBand-to-PCI Host Bridge";
        case 0x80: return "Other";
    }

    static char buffer[24];
    itoa(code, buffer, 16);
    return buffer;
}

static const char* DeviceClasses[] = 
{
    "Unclassified",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge Device",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device Controller",
    "Docking Station", 
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent Controller",
    "Satellite Communication Controller",
    "Encryption Controller",
    "Signal Processing Controller",
    "Processing Accelerator",
    "Non Essential Instrumentation"
};

const char* PCI_GetVendorName(uint16_t vendorID)
{
    switch (vendorID)
    {
        case 0x8086: return "Intel Corp.";
        case 0x1022: return "AMD";
        case 0x10DE: return "NVIDIA Corp.";
    }
    
    static char buffer[24];
    itoa(vendorID, buffer, 16);
    return buffer;
}

const char* PCI_GetDeviceName(uint16_t vendorID, uint16_t deviceID)
{
    switch (vendorID)
    {
        case 0x086:
            switch (deviceID)
            {
                case 0x29C0: return "Express DRAM Controller";
                case 0x2918: return "LPC Interface Controller";
                case 0x2922: return "6 port SATA Controller [ACHI MODE]";
                case 0x2930: return "SMBus Controller";
            }
    }

    static char buffer[24];
    itoa(deviceID, buffer, 16);
    return buffer;
}

const char* PCI_GetSubclassName(uint8_t classCode, uint8_t subclassCode)
{
   switch (classCode)
   {
        case 0x01:
            return MassStorageControllerName(subclassCode);
        case 0x03:
            switch (subclassCode)
            {
                case 0x00: return "VGA Compatible Controller";
            } break;

        case 0x06: return BridgeDeviceName(subclassCode);
        case 0x0C: return SerialBusControllerName(subclassCode);
    }

    static char buffer[24];
    itoa(subclassCode, buffer, 16);
    return buffer;
}

const char* PCI_GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF)
{
    switch (classCode)
    {
        case 0x01:
            switch (subclassCode)
            {
                case 0x06:
                    switch (progIF)
                    {
                        case 0x00: return "Vendor Specific Interface";
                        case 0x01: return "AHCI 1.0";
                        case 0x02: return "Serial Storage Bus";
                    } break;
            } break;
        case 0x03:
            switch (subclassCode)
            {
                case 0x00:
                    switch (progIF)
                    {
                        case 0x00: return "VGA Controller";
                        case 0x01: return "8514-Compatible Controller";
                    } break;
            } break;
        case 0x0C:
            switch (subclassCode)
            {
                case 0x03:
                    switch (progIF)
                    {
                        case 0x00: return "UHCI Controller";
                        case 0x10: return "OHCI Controller";
                        case 0x20: return "EHCI (USB2) Controller";
                        case 0x30: return "XHCI (USB3) Controller";
                        case 0x80: return "Unspecified";
                        case 0xFE: return "USB Device (Not a Host Controller)";
                    } break;
            } break;
    }

    static char buffer[24];
    itoa(progIF, buffer, 16);
    return buffer;
}

static void EnumerateFunction(uint64_t deviceAddress, uint64_t function)
{
    uint64_t offset = function << 12;
    uint64_t functionAddress = deviceAddress+offset;

    PageTableManager_MapMemory((void*)functionAddress, (void*)functionAddress);
    PCI_DeviceHeader* header = (PCI_DeviceHeader*)functionAddress;

    if (header->deviceID == 0)
        return;
    if (header->deviceID == 0xFFFF)
        return;
    
    Console_PrintString(PCI_GetVendorName(header->vendorID));
    Console_PrintString(" : ");
    Console_PrintString(PCI_GetDeviceName(header->vendorID, header->deviceID));
    Console_PrintString(" : ");
    Console_PrintString(DeviceClasses[header->class]);
    Console_PrintString(" : ");
    Console_PrintString(PCI_GetSubclassName(header->class, header->subclass));
    Console_PrintString(" : ");
    Console_PrintString(PCI_GetProgIFName(header->class, header->subclass, header->progInterfaceByte));
    Console_NewLine();
}

static void EnumerateDevice(uint64_t busAddress, uint64_t device)
{
    uint64_t offset = device << 15;
    uint64_t deviceAddress = busAddress+offset;

    PageTableManager_MapMemory((void*)deviceAddress, (void*)deviceAddress);
    PCI_DeviceHeader* header = (PCI_DeviceHeader*)deviceAddress;

    if (header->deviceID == 0)
        return;
    if (header->deviceID == 0xFFFF)
        return;

    for (uint64_t function = 0; function < 8; function++)
        EnumerateFunction(deviceAddress, function);
}

static void EnumerateBUS(uint64_t baseAddress, uint64_t bus)
{
    uint64_t offset = bus << 20;
    uint64_t busAddress = baseAddress+offset;

    PageTableManager_MapMemory((void*)busAddress, (void*)busAddress);
    PCI_DeviceHeader* header = (PCI_DeviceHeader*)busAddress;

    if (header->deviceID == 0)
        return;
    if (header->deviceID == 0xFFFF)
        return;

    for (uint64_t device = 0; device < 32; device++)
        EnumerateDevice(busAddress, device);
}

void EnumeratePCI(ACPI_MCFGHeader* mcfg)
{
    int entries = ((mcfg->header.size) - sizeof(ACPI_MCFGHeader)) / sizeof(ACPI_DeviceConfig);

    for (int i = 0; i < entries; ++i)
    {
        ACPI_DeviceConfig* deviceConfig = (ACPI_DeviceConfig*)((uint64_t)mcfg + sizeof(ACPI_MCFGHeader)
                                                              + (sizeof(ACPI_DeviceConfig) * i));
        for (uint64_t bus = deviceConfig->startBUS; bus < deviceConfig->endBUS; bus++)
            EnumerateBUS(deviceConfig->baseAddress, bus);
    }
}
