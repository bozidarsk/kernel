#include <assert.h>

#include "kernel/ioport.h"
#include "drivers/e1000e.h"

#include "registers.h"

static void e1000e_write(PciBAR bar, uint16_t offset, uint32_t value) 
{
	if (bar.type == PCI_BAR_TYPE_MEMORY)
		*(uint32_t*)((uint64_t)bar.memory.address + (uint64_t)offset) = value;
	else 
	{
		uint16_t address = bar.io.address;
		outd(address + IOADDR, offset);
		outd(address + IODATA, value);
	}
}

static uint32_t e1000e_read(PciBAR bar, uint16_t offset) 
{
	if (bar.type == PCI_BAR_TYPE_MEMORY)
		return *(uint32_t*)((uint64_t)bar.memory.address + (uint64_t)offset);
	else 
	{
		uint16_t address = bar.io.address;
		outd(address + IOADDR, offset);
		return ind(address + IODATA);
	}
}

static uint16_t e1000e_eeprom_read(PciBAR bar, uint8_t address) 
{
	EEC eec;

	*(uint32_t*)(&eec) = e1000e_read(bar, address);

	assert(eec.EE_PRES);
}

void e1000e_initialize(NetworkDevice* dev) 
{
	assert(dev);
	assert(dev->driver.type == DRIVER_TYPE_NETWORK);
	assert(dev->driver.bus == DRIVER_BUS_PCI);

	PciBAR bar = dev->driver.source.pciDevice->BAR0;
}
