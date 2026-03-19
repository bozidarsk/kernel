#ifndef ASSERT_H
#define ASSERT_H

#include <stdio.h>

#define assert(x) ({ if (!(x)) { dprintf(2, "assertion for '%s' failed in '%s:%i': %s()\n", #x, __FILE__, __LINE__, __FUNCTION__); abort(); } })

#endif
