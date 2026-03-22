#include <stdcountof.h>
#include <assert.h>

#include "kernel/drivers.h"

#include "drivers/e1000e.h"

static NetworkDevice netdevs[5];

NetworkDevice* drivers_get_netdev(void) 
{
	for (size_t i = 0; i < countof(netdevs); i++)
		if (netdevs[i].driver.isLoaded)
			return &netdevs[i];

	return NULL;
}

void drivers_unload(Driver* driver) 
{
	driver->isLoaded = false;

	// potential cleanup
}

#define X(x) for (size_t i = 0; i < countof(x); i++) { if (!x[i].driver.isLoaded) { device = &x[i]; driver = &x[i].driver; } }

Driver* drivers_load_pci(PciGeneralDevice* pciDevice) 
{
	assert(pciDevice);

	void* device = NULL;
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
	driver->source.pciDevice = pciDevice;

	switch (*(uint32_t*)pciDevice) 
	{
		case 0x10d38086:
			driver->name = "e1000e";
			e1000e_initialize((NetworkDevice*)device);
			break;
	}

	driver->isLoaded = true;
	return driver;
}

#undef X
