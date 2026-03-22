#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

#include "bus/pci.h"

typedef enum 
{
	DRIVER_TYPE_NETWORK,
	DRIVER_TYPE_VIDEO,
	DRIVER_TYPE_MOUSE,
	DRIVER_TYPE_KEYBOARD,
	DRIVER_TYPE_DISK,
	DRIVER_TYPE_CONSOLE,
} DriverType;

typedef enum 
{
	DRIVER_BUS_PCI,
	DRIVER_BUS_PCIE,
	DRIVER_BUS_USB,
	DRIVER_BUS_SERIAL,
} DriverBus;

typedef struct 
{
	DriverType type;
	DriverBus bus;
	bool isLoaded;
	const char* name;
	union  
	{
		PciGeneralDevice* pciDevice;
	} source; // the argument that has been used by drivers_load_* to initialize this driver
} Driver;

typedef struct 
{
	Driver driver;
	void(*transceive)(void* data, size_t size);
	void*(*receive)();
} NetworkDevice;

NetworkDevice* drivers_get_netdev(void);
// get first video dev
// get first mouse dev
// get first ... dev

void drivers_unload(Driver* driver);
Driver* drivers_load_pci(PciGeneralDevice* pciDevice);
// Driver* drivers_load_usb(UsbDevice* usbDevice);

#define drivers_load(dev) _Generic(dev, \
	PciGeneralDevice*: drivers_load_pci(dev), \
	default: assert(!"Cannot load driver for unknown device type '" #dev "'.") \
)

#endif
