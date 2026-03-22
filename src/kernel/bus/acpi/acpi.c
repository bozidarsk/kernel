#include <stddef.h>
#include <assert.h>

#include "bus/acpi.h"

void acpi_enumerate_xsdt(const XSDT* xsdt, void(*callback)(SDT* header)) 
{
	assert(xsdt);
	assert(callback);

	assert(xsdt->header.type == SDT_TYPE_XSDT);

	int entries = (xsdt->header.size - sizeof(xsdt->header)) / sizeof(xsdt->pOtherHeaders[0]);

	for (int i = 0; i < entries; i++) 
	{
		SDT* header = (SDT*)xsdt->pOtherHeaders[i];
		callback(header);
	}
}

void acpi_enumerate_madt(const MADT* madt, void(*callback)(MADTEntry* entry)) 
{
	assert(madt);
	assert(callback);

	uint32_t max = madt->header.size - sizeof(MADT);
	MADTEntry* entry = NULL;

	for (uint32_t i = 0; i < max; i += entry->size) 
	{
		entry = (MADTEntry*)(madt->entries + i);
		callback(entry);
	}
}
