#ifndef DRIVERS_H
#define DRIVERS_H

#include <stdint.h>
#include <stdbool.h>
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

typedef struct Driver
{
	DriverType type;
	DriverBus bus;
	bool isLoaded;
	const char* name;
	union
	{
		PciGeneralDevice* asPci;
	} source; // the argument that has been used by drivers_load_* to initialize this driver
	void (*initialize)(struct Driver* driver);
	void (*free)(struct Driver* driver);
} Driver;

typedef struct
{
	Driver base;
	uint8_t mac[6];
	void(*transceive)(void* data, size_t size);
	void*(*receive)();
} NetworkDriver;

NetworkDriver* drivers_get_network(void);
// get first video dev
// get first mouse dev
// get first ... dev

void drivers_unload(Driver* driver);
Driver* drivers_load_pci(PciGeneralDevice* pciDevice);
// Driver* drivers_load_usb(UsbDevice* usbDevice);

#define drivers_load(device) _Generic(device, \
	PciGeneralDevice*: drivers_load_pci(device), \
	default: assert(!"Cannot load driver for unknown device type '" #device "'.") \
)

#endif
