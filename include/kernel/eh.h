#ifndef EH_H
#define EH_H

#include <stdio.h>

#define breakpoint() ({ __asm__ volatile("int3"); })
#define hlt() ({ while (true) __asm__ volatile("hlt"); })

#define throw(exception, message) \
({ \
	printf("exception in '%s:%i': %s(): %s - %s\n", __FILE__, __LINE__, __FUNCTION__, #exception, message); \
	breakpoint(); \
	hlt(); \
})

#endif
