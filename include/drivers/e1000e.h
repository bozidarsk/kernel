#ifndef DRIVERS_E1000E_H
#define DRIVERS_E1000E_H

#include <stdint.h>
#include <stdbool.h>

#include "kernel/drivers.h"

typedef struct
{
	NetworkDriver* driver;
	uint32_t address;
	bool useIo;
} e1000e;

uint32_t e1000e_read_mmio(uint32_t ioaddr, uint16_t offset);
void e1000e_write_mmio(uint32_t ioaddr, uint16_t offset, uint32_t value);
uint32_t e1000e_read_io(uint32_t ioaddr, uint16_t offset);
void e1000e_write_io(uint32_t ioaddr, uint16_t offset, uint32_t value);
uint32_t e1000e_read(e1000e* device, uint16_t offset);
void e1000e_write(e1000e* device, uint16_t offset, uint32_t value);

uint16_t e1000e_read_eeprom(e1000e* device, uint8_t address);

void e1000e_initialize(NetworkDriver* driver);

#endif
