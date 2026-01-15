#ifndef EH_H
#define EH_H

#include <stdio.h>

__attribute__((noreturn))
void halt();

void breakpoint();

#define throw(exception, message) \
({ \
	printf("exception in '%s:%i': %s(): %s - %s\n", __FILE__, __LINE__, __FUNCTION__, #exception, message); \
	breakpoint(); \
	halt(); \
})

#endif
