#include "kernel/pci.h"
#include "kernel/ioport.h"
#include "kernel/eh.h"

uint32_t pci_read_uint32(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) 
{
	outd(PCI_CONFIG_ADDRESS,
		((uint32_t)1 << 31) | ((uint32_t)bus << 16) | ((uint32_t)device << 11) | ((uint32_t)function << 8) | ((uint32_t)offset & ~0b11)
	);

	return ind(PCI_CONFIG_DATA);
}

uint16_t pci_read_uint16(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) 
{
	uint32_t data = pci_read_uint32(bus, device, function, offset);

	return (data >> ((offset & 0b10) * 8)) & 0xffff;
}

uint8_t pci_read_uint8(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) 
{
	uint32_t data = pci_read_uint32(bus, device, function, offset);

	return (data >> ((offset & 0b11) * 8)) & 0xff;
}

void pci_read_header(uint8_t bus, uint8_t device, uint8_t function, DeviceHeader* header) 
{
	assert(header);

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
	header->type = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	*(uint8_t*)(&header->bist) = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
}

void pci_read_general_device(uint8_t bus, uint8_t device, uint8_t function, GeneralDevice* generalDevice) 
{
	assert(generalDevice);

	pci_read_header(bus, device, function, &generalDevice->header);

	uint8_t offset = sizeof(DeviceHeader);

	*(uint32_t*)(&generalDevice->BAR0) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	*(uint32_t*)(&generalDevice->BAR1) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	*(uint32_t*)(&generalDevice->BAR2) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	*(uint32_t*)(&generalDevice->BAR3) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	*(uint32_t*)(&generalDevice->BAR4) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	*(uint32_t*)(&generalDevice->BAR5) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	generalDevice->cardbusCisPointer = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	generalDevice->subsystemVendor = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	generalDevice->subsystem = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	generalDevice->expansionROMBase = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	generalDevice->capabilitiesPointer = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	offset += 7; // reserved
	generalDevice->interruptLine = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	generalDevice->interruptPin = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	generalDevice->minGrant = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	generalDevice->maxLatency = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
}

void pci_read_pci_brigde_device(uint8_t bus, uint8_t device, uint8_t function, PCIBridgeDevice* pciBridgeDevice) 
{
	assert(pciBridgeDevice);

	pci_read_header(bus, device, function, &pciBridgeDevice->header);

	uint8_t offset = sizeof(DeviceHeader);

	*(uint32_t*)(&pciBridgeDevice->BAR0) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	*(uint32_t*)(&pciBridgeDevice->BAR1) = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	pciBridgeDevice->primaryBus = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	pciBridgeDevice->secondaryBus = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	pciBridgeDevice->subordinateBus = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	pciBridgeDevice->secondaryLatencyTimer = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	pciBridgeDevice->ioBase = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	pciBridgeDevice->ioLimit = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	*(uint16_t*)(&pciBridgeDevice->secondaryStatus) = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	pciBridgeDevice->memoryBase = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	pciBridgeDevice->memoryLimit = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	pciBridgeDevice->prefetchableMemoryBase = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	pciBridgeDevice->prefetchableMemoryLimit = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	pciBridgeDevice->prefetchableMemoryBaseUpper = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	pciBridgeDevice->prefetchableMemoryLimitUpper = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	pciBridgeDevice->ioBaseUpper = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	pciBridgeDevice->ioLimitUpper = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	pciBridgeDevice->capabilitiesPointer = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	offset += 3; // reserved
	pciBridgeDevice->expansionROMBase = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	pciBridgeDevice->interruptLine = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	pciBridgeDevice->interruptPin = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	pciBridgeDevice->bridgeControl = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
}

void pci_read_cardbus_brigde_device(uint8_t bus, uint8_t device, uint8_t function, CardbusBridgeDevice* cardbusBridgeDevice) 
{
	assert(cardbusBridgeDevice);

	pci_read_header(bus, device, function, &cardbusBridgeDevice->header);

	uint8_t offset = sizeof(DeviceHeader);

	cardbusBridgeDevice->socketBase = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->capabilitiesPointer = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	cardbusBridgeDevice->reserved = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	*(uint16_t*)(&cardbusBridgeDevice->secondaryStatus) = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	cardbusBridgeDevice->pciBus = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	cardbusBridgeDevice->cardbusBus = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	cardbusBridgeDevice->subordinateBus = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	cardbusBridgeDevice->cardbusLatencyTimer = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	cardbusBridgeDevice->memoryBaseLower = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->memoryLimitLower = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->memoryBaseUpper = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->memoryLimitUpper = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->ioBaseLower = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->ioLimitLower = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->ioBaseUpper = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->ioLimitUpper = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
	cardbusBridgeDevice->interruptLine = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	cardbusBridgeDevice->interruptPin = pci_read_uint8(bus, device, function, offset); offset += sizeof(uint8_t);
	cardbusBridgeDevice->bridgeControl = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	cardbusBridgeDevice->subsystemDevice = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	cardbusBridgeDevice->subsystemVendor = pci_read_uint16(bus, device, function, offset); offset += sizeof(uint16_t);
	cardbusBridgeDevice->pcCardLegacyBase = pci_read_uint32(bus, device, function, offset); offset += sizeof(uint32_t);
}

bool pci_exists(uint8_t bus, uint8_t device, uint8_t function, DeviceType* type, bool* hasMultipleFunctions) 
{
	uint8_t value = pci_read_uint8(bus, device, function, 14);

	if (type) *type = value & 0b01111111;
	if (hasMultipleFunctions) *hasMultipleFunctions = value >> 7;

	return pci_read_uint16(bus, device, function, 0) != 0xffff;
}

void pci_enumerate_devices(void(*callback)(DeviceHeader* header)) 
{
	for (int bus = 0; bus < 256; bus++) 
	{
		for (int device = 0; device < 32; device++) 
		{
			int function = 0;
			DeviceType type;
			bool hasMultipleFunctions;

			do 
			{
				if (!pci_exists(bus, device, function, &type, &hasMultipleFunctions))
					continue;

				switch (type) 
				{
					case PCI_DEVICE_TYPE_GENERAL:
						GeneralDevice generalDevice;
						pci_read_general_device(bus, device, function, &generalDevice);
						callback(&generalDevice.header);
						break;
					case PCI_DEVICE_TYPE_PCI_BRIDGE:
						PCIBridgeDevice pciBridgeDevice;
						pci_read_pci_brigde_device(bus, device, function, &pciBridgeDevice);
						callback(&pciBridgeDevice.header);
						break;
					case PCI_DEVICE_TYPE_CARDBUS_BRIDGE:
						CardbusBridgeDevice cardbusBridgeDevice;
						pci_read_cardbus_brigde_device(bus, device, function, &cardbusBridgeDevice);
						callback(&cardbusBridgeDevice.header);
						break;
					default:
						assert(!"Unknown pci device type.");
				}

				function++;
			} while (hasMultipleFunctions);
		}
	}
}
