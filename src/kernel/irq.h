#pragma once

#include "isr.h"
#include "pic.h"
#include "io.h"

#define PIC_REMAP_OFFSET	0x20

typedef void (*IRQ_Handler)(Registers* regs);

void init_iqr();
extern void iqr_register_handler(int iqr, IRQ_Handler handler);