#include <stdcountof.h>
#include <assert.h>

#include "kernel/drivers.h"

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

	// potential cleanup (ex. ((NetworkDevice*)driver)->free() )
}

#define X(x) for (size_t i = 0; i < countof(x); i++) { if (!x[i].driver.isLoaded) driver = &x[i].driver; }

Driver* drivers_load_pci(PciDeviceHeader* header) 
{
	assert(header);

	Driver* driver = NULL;
	switch (header->class) 
	{
		case PCI_DEVICE_CLASS_NETWORK_CONTROLLER:
			X(netdevs)
			break;
	}

	if (!driver)
		return NULL;

	// check vendor and device and match to corresponding driver (ex. e1000e)

	uint32_t id = *(uint32_t*)header;
	switch (id) 
	{
		case 0x10d38086:
			driver->name = "e1000e";
			// init
			break;
	}

	driver->isLoaded = true;
	return driver;
}

#undef X
