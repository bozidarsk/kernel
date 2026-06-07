#include <assert.h>
#include <stdint.h>
#include <stdio.h>

#include "bus/pci.h"
#include "kernel/drivers.h"
#include "kernel/ioport.h"
#include "drivers/e1000e.h"

#include "registers.h"

uint32_t e1000e_read_mmio(uint32_t ioaddr, uint16_t offset)
{
	return *(uint32_t*)((uint64_t)ioaddr + offset);
}

void e1000e_write_mmio(uint32_t ioaddr, uint16_t offset, uint32_t value)
{
	*(uint32_t*)((uint64_t)ioaddr + offset) = value;
}

uint32_t e1000e_read_io(uint32_t ioaddr, uint16_t offset)
{
	outd(ioaddr + IOADDR, offset);
	return ind(ioaddr + IODATA);
}

void e1000e_write_io(uint32_t ioaddr, uint16_t offset, uint32_t value)
{
	outd(ioaddr + IOADDR, offset);
	outd(ioaddr + IODATA, value);
}

uint32_t e1000e_read(e1000e* device, uint16_t offset)
{
	return (device->useIo) ? e1000e_read_io(device->address, offset) : e1000e_read_mmio(device->address, offset);
}

void e1000e_write(e1000e* device, uint16_t offset, uint32_t value)
{
	return (device->useIo) ? e1000e_write_io(device->address, offset, value) : e1000e_write_mmio(device->address, offset, value);
}

uint16_t e1000e_read_eeprom(e1000e* device, uint8_t address)
{
	EEC eec;
	EERD eerd;

	eec = e1000e_read_eec(device);
	assert(eec.EE_PRES);

	eerd.START = 1;
	eerd.ADDR = address;

	e1000e_write_eerd(device, eerd);

	do
	{
		eerd = e1000e_read_eerd(device);
	} while (!eerd.DONE);

	return eerd.DATA;
}

void e1000e_reset(e1000e* device)
{
	CTRL ctrl;

	ctrl = e1000e_read_ctrl(device);
	ctrl.RST = 1;
	e1000e_write_ctrl(device, ctrl);

	do
	{
		ctrl = e1000e_read_ctrl(device);
		printf("AAA\n");
	} while (ctrl.RST);

	ctrl.SLU = 1;
	ctrl.FRCSPD = 0;

	e1000e_write_ctrl(device, ctrl);

	uint16_t b0 = e1000e_read_eeprom(device, 0);
	uint16_t b1 = e1000e_read_eeprom(device, 1);
	uint16_t b2 = e1000e_read_eeprom(device, 2);

	uint32_t writeLow = ((uint32_t)b1 << 16) | b0;
	uint32_t writeHigh = b2;

	e1000e_write(device, REGISTER_RAL, writeLow);
	e1000e_write(device, REGISTER_RAH, writeHigh);

	device->driver->mac[0] = (b0 >> 0) & 0xff;
	device->driver->mac[1] = (b0 >> 8) & 0xff;
	device->driver->mac[2] = (b1 >> 0) & 0xff;
	device->driver->mac[3] = (b1 >> 8) & 0xff;
	device->driver->mac[4] = (b2 >> 0) & 0xff;
	device->driver->mac[5] = (b2 >> 8) & 0xff;
}

void e1000e_initialize(NetworkDriver* driver)
{
	assert(driver);
	assert(driver->base.type == DRIVER_TYPE_NETWORK);

	e1000e device;
	device.driver = driver;

	switch (driver->base.bus)
	{
		case DRIVER_BUS_PCI:
			device.address = pci_bar_decode(driver->base.source.asPci->BAR0, driver->base.source.asPci->BAR1);
			device.useIo = driver->base.source.asPci->BAR0.type == PCI_BAR_TYPE_IO;
			break;
		default:
			assert(!"Invalid bus type.");
	}

	e1000e_reset(&device);

	printf("%02x:%02x:%02x:%02x:%02x:%02x\n", driver->mac[0], driver->mac[1], driver->mac[2], driver->mac[3], driver->mac[4], driver->mac[5]);

	printf("e1000e: DONE\n");
}
