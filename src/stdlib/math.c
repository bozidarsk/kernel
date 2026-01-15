#include <stdint.h>

double fabs(double x) 
{
	uint64_t memory = *(uint64_t*)&x;
	memory &= (1ul << 63) - 1;
	return *(double*)&memory;
}

double floor(double x) 
{
	uint64_t memory = *(uint64_t*)&x;

	return (memory < 0.0) ? (double)(int64_t)x - 1.0 : (double)(int64_t)x;
}
