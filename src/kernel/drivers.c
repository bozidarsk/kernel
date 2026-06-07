#include <stdcountof.h>
#include <assert.h>

#include "kernel/drivers.h"

#include "drivers/e1000e.h"

static NetworkDriver netdevs[5];

NetworkDriver* drivers_get_netdev(void)
{
	for (size_t i = 0; i < countof(netdevs); i++)
		if (netdevs[i].base.isLoaded)
			return &netdevs[i];

	return NULL;
}

void drivers_unload(Driver* driver)
{
	driver->isLoaded = false;

	if (driver->free)
		driver->free(driver);
}

#define X(x) for (size_t i = 0; i < countof(x); i++) { if (!x[i].base.isLoaded) { driver = &x[i].base; } }

Driver* drivers_load_pci(PciGeneralDevice* pciDevice)
{
	assert(pciDevice);

	Driver* driver = NULL;

	switch (pciDevice->header.class)
	{
		case PCI_DEVICE_CLASS_NETWORK_CONTROLLER:
			X(netdevs)
			if (driver) driver->type = DRIVER_TYPE_NETWORK;
			break;
	}

	if (!driver)
		return NULL;

	driver->bus = DRIVER_BUS_PCI;
	driver->source.asPci = pciDevice;

	switch (*(uint32_t*)pciDevice)
	{
		case 0x10d38086:
			driver->name = "e1000e";
			driver->initialize = (void(*)(Driver*))&e1000e_initialize;
			break;
	}

	driver->isLoaded = true;
	driver->initialize(driver);

	return driver;
}

#undef X
