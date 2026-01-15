#include "kernel/pci.h"
#include "kernel/ioport.h"
#include "kernel/eh.h"

uint32_t pci_read_uint32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) 
{
	outd(PCI_CONFIG_ADDRESS,
		(uint32_t)(1 << 31) | (uint32_t)(bus << 16) | (uint32_t)(device << 11) | (uint32_t)(function << 8) | (uint32_t)(offset & ~0b11)
	);

	return ind(PCI_CONFIG_DATA);
}

uint16_t pci_read_uint16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) 
{
	uint32_t data = pci_read_uint32(bus, device, function, offset);

	return (offset % 4 != 0)
		? (uint16_t)(data >> 16)
		: (uint16_t)data
	;
}

uint8_t pci_read_uint8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) 
{
	uint32_t data = pci_read_uint32(bus, device, function, offset);

	return (offset % 2 != 0)
		? (uint8_t)(data >> 8)
		: (uint8_t)data
	;
}

void pci_read_header(uint8_t bus, uint8_t device, uint8_t function, CommonHeader* header) 
{
	if (!header)
		throw(ArgumentNullException, "Header pointer cannot be null.");

	uint8_t offset = 0;

	header->vendor = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	header->device = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	*(uint16_t*)(&(header->command)) = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	*(uint16_t*)(&(header->status)) = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	header->revision = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	header->progif = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	header->subclass = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	header->cacheLineSize = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	header->latencyTimer = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	header->headerType = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	header->bist = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
}

bool pci_exists(uint8_t bus, uint8_t device, uint8_t function, HeaderType* type) 
{
	if (type)
		*type = pci_read_uint16(bus, device, function, 14) & 0b01111111;

	return pci_read_uint16(bus, device, function, 0) != 0xffff;
}

void pci_enumerate_devices(void(*func)(CommonHeader header)) 
{
	for (int bus = 0; bus < 256; bus++) 
	{
		for (int device = 0; device < 32; device++) 
		{
			uint8_t function = 0;
			HeaderType type;

			if (!pci_exists(bus, device, function, &type))
				continue;

			if (type != PCI_HEADER_TYPE_GENERAL)
				continue;

			CommonHeader header;
			pci_read_header(bus, device, function, &header);

			func(header);
		}
	}
}
