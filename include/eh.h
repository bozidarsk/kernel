#ifndef EH_H
#define EH_H

#include "console.h"

#define breakpoint() ({ __asm__ volatile("int3"); })
#define hlt() ({ while (true) __asm__ volatile("hlt"); })

#define throw(exception, message) \
({ \
	console_write_string("exception in '"); \
	console_write_string(__FILE__); \
	console_write_string("': "); \
	console_write_string(__FUNCTION__); \
	console_write_string(": "); \
	console_write_string(#exception); \
	console_write_string(": "); \
	console_write_string(message); \
	console_write_string("\n"); \
	breakpoint(); \
	hlt(); \
})

#endif
