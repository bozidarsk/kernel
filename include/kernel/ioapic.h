#ifndef IOAPIC_H
#define IOAPIC_H

#include <stdint.h>

uint32_t ioapic_register_read(int offset);
void ioapic_register_write(int offset, uint32_t value);

#endif
