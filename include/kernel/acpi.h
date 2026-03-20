#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

typedef enum : uint32_t
{
	SDT_TYPE_APIC = 0x43495041, // Multiple APIC Description Table (MADT)
	SDT_TYPE_BGRT = 0x54524742, // Boot Graphics Resource Table (BGRT; only supported on UEFI systems)
	SDT_TYPE_BERT = 0x54524542, // Boot Error Record Table (BERT)
	SDT_TYPE_CPEP = 0x50455043, // Corrected Platform Error Polling Table (CPEP)
	SDT_TYPE_DSDT = 0x54445344, // Differentiated System Description Table (DSDT)
	SDT_TYPE_ECDT = 0x54444345, // Embedded Controller Boot Resources Table (ECDT)
	SDT_TYPE_EINJ = 0x4a4e4945, // Error Injection Table (EINJ)
	SDT_TYPE_ERST = 0x54535245, // Error Record Serialization Table (ERST)
	SDT_TYPE_FACP = 0x50434146, // Fixed ACPI Description Table (FADT)
	SDT_TYPE_FACS = 0x53434146, // Firmware ACPPI Control Structure (FACS)
	SDT_TYPE_HEST = 0x54534548, // Hardware Error Source Table (HEST)
	SDT_TYPE_MSCT = 0x5443534d, // Maximum System Characteristics Table (MSCT)
	SDT_TYPE_MPST = 0x5453504d, // Memory Power State Table (MPST)
	SDT_TYPE_PMTT = 0x54544d50, // Platform Memory Topology Table (PMTT)
	SDT_TYPE_PSDT = 0x54445350, // Persistent System Description Table (PSDT)
	SDT_TYPE_RASF = 0x46534152, // ACPI RAS FeatureTable (RASF)
	SDT_TYPE_RSDT = 0x54445352, // Root System Description Table (RSDT; 32-bit version of the XSDT)
	SDT_TYPE_SBST = 0x54534253, // Smart Battery Specification Table (SBST)
	SDT_TYPE_SLIT = 0x54494c53, // System Locality System Information Table (SLIT)
	SDT_TYPE_SRAT = 0x54415253, // System Resource Affinity Table (SRAT)
	SDT_TYPE_SSDT = 0x54445353, // Secondary System Description Table (SSDT)
	SDT_TYPE_XSDT = 0x54445358, // Extended System Description Table (XSDT)
	SDT_TYPE_OEM  = 0x004d454f, // OEM Specific Information Tables (Any table with a signature beginning with "OEM" falls into this definition)
} SDTType;

typedef struct 
{
	SDTType type;
	uint32_t size;
	uint8_t revision;
	uint8_t checksum;
	uint8_t oemId[6];
	uint8_t oemTableId[8];
	uint32_t oemRevision;
	uint32_t creatorId;
	uint32_t creatorRevision;
} __attribute__((packed)) SDT;

typedef struct 
{
	SDT header;
	uint64_t pOtherHeaders[0];
} __attribute__((packed)) XSDT; // provided by bootloader

typedef struct 
{
	uint8_t signature[8];
	uint8_t checksum;
	uint8_t oemId[6];
	uint8_t revision;
	__attribute__((deprecated)) uint32_t rsdtAddress;

	uint32_t size;
	uint64_t xsdtAddress;
	uint8_t extendedChecksum;
	uint8_t reserved[3];
} __attribute__((packed)) XSDP;

typedef enum : uint8_t
{
	MADT_ENTRY_TYPE_PROCESSOR_LOCAL_APIC = 0,
	MADT_ENTRY_TYPE_IO_APIC = 1,
	MADT_ENTRY_TYPE_IO_APIC_INTERRUPT_SOURCE_OVERRIDE = 2,
	MADT_ENTRY_TYPE_IO_APIC_NON_MASKABLE_INTERRUPT = 3,
	MADT_ENTRY_TYPE_LOCAL_APIC_NON_MASKABLE_INTERRUPT = 4,
	MADT_ENTRY_TYPE_LOCAL_APIC_ADDRESS_OVERRIDE = 5,
	MADT_ENTRY_TYPE_PROCESSOR_LOCAL_x2APIC = 9,
} MADTEntryType;

typedef enum 
{
	MADT_ENTRY_FLAGS_DEFAULT          = 0b0000,
	MADT_ENTRY_FLAGS_ACTIVE_HIGH      = 0b0001,
	MADT_ENTRY_FLAGS_ACTIVE_LOW       = 0b0011,
	MADT_ENTRY_FLAGS_EDGE_TRIGGERD    = 0b0100,
	MADT_ENTRY_FLAGS_LEVEL_TRIGGERED  = 0b1100,
} MADTEntryFlags;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
} __attribute__((packed)) MADTEntry;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
	uint8_t processorId;
	uint8_t apicId;
	uint32_t processorEnabled : 1;
	uint32_t onlineCapable : 1;
} __attribute__((packed)) MADTEntryProcessorLocalAPIC;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
	uint8_t id;
	uint8_t reserved;
	uint32_t address;
	uint32_t globalSystemInterruptBase;
} __attribute__((packed)) MADTEntryIOAPIC;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
	uint8_t bus;
	uint8_t irq;
	uint32_t globalSystemInterrupt;
	uint16_t flags;
} __attribute__((packed)) MADTEntryIOAPICInterruptSourceOverride;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
	uint8_t nmi;
	uint8_t	reserved;
	uint16_t flags;
	uint32_t globalSystemInterrupt;
} __attribute__((packed)) MADTEntryIOAPICNonMaskableInterrupt;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
	uint8_t processorId;
	uint16_t flags;
	uint8_t lintn;
} __attribute__((packed)) MADTEntryLocalAPICNonMaskableInterrupt;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
	uint16_t reserved;
	uint64_t address;
} __attribute__((packed)) MADTEntryLocalAPICAddressOverride;

typedef struct 
{
	MADTEntryType type;
	uint8_t size;
	uint16_t reserved;
	uint32_t processorId;
	uint32_t processorEnabled : 1;
	uint32_t onlineCapable : 1;
	uint32_t acpiId;
} __attribute__((packed)) MADTEntryProcessorLocalx2APIC;

typedef struct 
{
	SDT header;
	uint32_t localAPICAddress;
	uint32_t flags;
	uint8_t entries[0];
} __attribute__((packed)) MADT;

void acpi_enumerate_xsdt(const XSDT* xsdt, void(*callback)(SDT* header));
void acpi_enumerate_madt(const MADT* madt, void(*callback)(MADTEntry* entry));

#endif
