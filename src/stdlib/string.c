#include "stdlib.h"

size_t strlen(const char* str) 
{
	if (!str)
		return 0;

	size_t length = 0;
	while (str[length]) length++;

	return length;
}

int strcmp(const char* a, const char* b) 
{
	if (!a && !b)
		return 0;

	if (!a && b)
		return -1;

	if (a && !b)
		return 1;

	for (size_t i = 0; a[i] || b[i]; i++) 
	{
		int diff = a[i] - b[i];
		if (diff) return diff;
	}

	return 0;
}

int strncmp(const char* a, const char* b, size_t n) 
{
	if (!a && !b)
		return 0;

	if (!a && b)
		return -1;

	if (a && !b)
		return 1;

	for (size_t i = 0; (a[i] || b[i]) && i < n; i++) 
	{
		int diff = a[i] - b[i];
		if (diff) return diff;
	}

	return 0;
}
