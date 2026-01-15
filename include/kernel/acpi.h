#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>

typedef struct  
{
	uint8_t signature[4];
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

#endif
