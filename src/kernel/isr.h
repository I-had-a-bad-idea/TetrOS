#pragma once

#include "stdint.h"
#include "kernel.h"
#include "idt.h"

typedef struct Registers {
  uint32_t ds;
  uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  // pushed by pusha
  uint32_t interrupt_number, error_code;  // interrupt number and error code
  uint32_t eip, cs, eflags, useresp, ss;  // pushed by the processor automatically
} __attribute__((packed)) Registers;

typedef void (*ISR_Handler)(Registers* regs);

void init_isr();
extern void isr_register_handler(int interrup_number, ISR_Handler handler);