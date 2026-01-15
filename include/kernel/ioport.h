#ifndef IOPORT_H
#define IOPORT_H

#include <stdint.h>

void outd(uint16_t port, uint32_t value);
void outw(uint16_t port, uint16_t value);
void outb(uint16_t port, uint8_t value);
uint32_t ind(uint16_t port);
uint16_t inw(uint16_t port);
uint8_t inb(uint16_t port);

void iowait(void);

#endif
