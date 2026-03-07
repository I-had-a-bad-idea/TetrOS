#pragma once

#include "stdint.h"

#define UNUSED_PORT 0x80

void __attribute__((cdecl)) outb(uint16_t port, uint8_t value);
uint8_t __attribute__((cdecl)) inb(uint16_t port);


void io_wait();