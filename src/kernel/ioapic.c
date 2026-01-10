#include "ioapic.h"
#include "apic.h"

uint32_t ioapic_register_read(int offset)
{
	void* base = apic_get_base();

	/* tell IOREGSEL where we want to read from */
	*(volatile uint32_t*)base = offset;
	/* return the data from IOWIN */
	return *(volatile uint32_t*)((volatile uint8_t*)base + 0x10);
}

void ioapic_register_write(int offset, uint32_t value) 
{
	void* base = apic_get_base();

	/* tell IOREGSEL where we want to write to */
	*(volatile uint32_t*)base = offset;
	/* write the value to IOWIN */
	*(volatile uint32_t*)((volatile uint8_t*)base + 0x10) = value; 
}
