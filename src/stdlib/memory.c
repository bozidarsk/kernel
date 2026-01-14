#include "stdlib.h"

void* memcpy(void* destination, const void* source, size_t size) 
{
	if (!destination || !source)
		return destination;

	for (size_t i = 0; i < size; i++)
		((uint8_t*)destination)[i] = ((uint8_t*)source)[i];

	return destination;
}

void* memset(void* destination, int32_t value, size_t size) 
{
	if (!destination)
		return destination;

	for (size_t i = 0; i < size; i++)
		((uint8_t*)destination)[i] = (uint8_t)value;

	return destination;
}
