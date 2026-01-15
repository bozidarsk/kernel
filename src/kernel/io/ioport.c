#include "kernel/ioport.h"

void outd(uint16_t port, uint32_t value) { __asm__ volatile("out %0, %1" : : "a" (value), "d" (port)); }
void outw(uint16_t port, uint16_t value) { __asm__ volatile("out %0, %1" : : "a" (value), "d" (port)); }
void outb(uint16_t port, uint8_t value) { __asm__ volatile("out %0, %1" : : "a" (value), "d" (port)); }

uint32_t ind(uint16_t port) { uint32_t value; __asm__ volatile("in %1, %0" : "=a" (value) : "d" (port)); return value; }
uint16_t inw(uint16_t port) { uint16_t value; __asm__ volatile("in %1, %0" : "=a" (value) : "d" (port)); return value; }
uint8_t inb(uint16_t port) { uint8_t value; __asm__ volatile("in %1, %0" : "=a" (value) : "d" (port)); return value; }
