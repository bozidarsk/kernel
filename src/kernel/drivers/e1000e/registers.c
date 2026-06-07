#include "drivers/e1000e.h"
#include "registers.h"

#define X(r, R) \
R e1000e_read_ ## r (e1000e* device) \
{ \
	uint32_t value = e1000e_read(device, REGISTER_ ## R); \
	return *(R*)(&value); \
} \
void e1000e_write_ ## r (e1000e* device, R r) \
{ \
	e1000e_write(device, REGISTER_ ## R, *(uint32_t*)&r); \
}

X(ctrl, CTRL)
X(eec, EEC)
X(eerd, EERD)
X(ims, IMS)
X(ral, RAL)
X(rah, RAH)

#undef X
